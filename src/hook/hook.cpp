#include "hook.h"
#include "../dummy/dummy.h"
#include "../ui/menu.h"
#include <MinHook.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

// vTable
using tPresent = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);
using tResizeBuffers = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

static tPresent oPresent = nullptr;
static tResizeBuffers oResizeBuffers = nullptr;
static void* presentAddr = nullptr;
static void* resizeAddr = nullptr;

static ID3D11Device* g_Device = nullptr;
static ID3D11DeviceContext* g_Context = nullptr;
static ID3D11RenderTargetView* g_RTV = nullptr;
static HWND g_Hwnd = nullptr;
static WNDPROC oWndProc = nullptr;
static bool g_ImGuiInit = false;

// Enables mouse/keyboard input in ImGui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT WINAPI HookedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYUP && wParam == VK_F1)
        Menu_Toggle();

    // Let ImGui process input first when the menu is visible.
    if (Menu_IsVisible() && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return 1;

    return CallWindowProcA(oWndProc, hWnd, msg, wParam, lParam);
}

static void CreateRenderTarget(IDXGISwapChain* pSwapChain)
{
    if (!g_Device || g_RTV)
        return;
    ID3D11Texture2D* backBuffer = nullptr;
    if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))) && backBuffer)
    {
        g_Device->CreateRenderTargetView(backBuffer, nullptr, &g_RTV);
        backBuffer->Release();
    }
}

static void CleanupRenderTarget()
{
    if (!g_RTV)
        return;
    if (g_Context)
        g_Context->OMSetRenderTargets(0, nullptr, nullptr);
    g_RTV->Release();
    g_RTV = nullptr;
}

static bool InitImGui(IDXGISwapChain* pSwapChain)
{
    if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&g_Device))))
        return false;
    g_Device->GetImmediateContext(&g_Context);

    DXGI_SWAP_CHAIN_DESC sd = {};
    pSwapChain->GetDesc(&sd);
    g_Hwnd = sd.OutputWindow;

    CreateRenderTarget(pSwapChain);

    ImGui::CreateContext();
    Menu_InitStyle();
    ImGui_ImplWin32_Init(g_Hwnd);
    ImGui_ImplDX11_Init(g_Device, g_Context);

    oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(g_Hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HookedWndProc)));
    g_ImGuiInit = true;
    return true;
}

static HRESULT STDMETHODCALLTYPE HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!g_ImGuiInit)
    {
        if (!InitImGui(pSwapChain))
            return oPresent(pSwapChain, SyncInterval, Flags);
    }

    if (!g_RTV)
        CreateRenderTarget(pSwapChain);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    // Imgui's stuff
    ImGui::NewFrame();

    Menu_Render();

    ImGui::Render();
    // 
    g_Context->OMSetRenderTargets(1, &g_RTV, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, SyncInterval, Flags);
}

static HRESULT STDMETHODCALLTYPE HookedResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT Format, UINT Flags)
{
    if (oResizeBuffers)
        CleanupRenderTarget();
    if (oResizeBuffers)
        return oResizeBuffers(pSwapChain, BufferCount, Width, Height, Format, Flags);
    return S_OK;
}

bool PresentHook()
{
    if (MH_Initialize() != MH_OK)
        return false;

    if (!GetPresentAddress(&presentAddr))
    {
        MH_Uninitialize();
        return false;
    }

    GetResizeBuffersAddress(&resizeAddr);

    if (MH_CreateHook(presentAddr, reinterpret_cast<LPVOID>(&HookedPresent), reinterpret_cast<LPVOID*>(&oPresent)) != MH_OK)
    {
        MH_Uninitialize();
        return false;
    }

    if (resizeAddr)
    {
        if (MH_CreateHook(resizeAddr, reinterpret_cast<LPVOID>(&HookedResizeBuffers), reinterpret_cast<LPVOID*>(&oResizeBuffers)) != MH_OK)
        {
            resizeAddr = nullptr;
        }
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        MH_Uninitialize();
        return false;
    }

    return true;
}

void PresentUnhook()
{
    if (g_Hwnd && oWndProc)
    {
        SetWindowLongPtrA(g_Hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
        oWndProc = nullptr;
        g_Hwnd = nullptr;
    }

    if (g_ImGuiInit)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        g_ImGuiInit = false;
    }

    if (g_RTV) { g_RTV->Release(); g_RTV = nullptr; }
    if (g_Context) { g_Context->Release(); g_Context = nullptr; }
    if (g_Device) { g_Device->Release(); g_Device = nullptr; }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    presentAddr = nullptr;
    resizeAddr = nullptr;
    oPresent = nullptr;
    oResizeBuffers = nullptr;
}
