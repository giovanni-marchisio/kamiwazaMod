#include "hook.h"
#include "../dummy/dummy.h"
#include "../MinHook/MinHook.h"
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

// vTable
using tPresent = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);
static tPresent oPresent = nullptr;
static void* presentAddr = nullptr;

// In-game func stuff
constexpr uintptr_t INV_REMOVE_RVA = 0xE099E0; // FUN_140e099e0
constexpr ULONGLONG ITEM_GATE_MS = 1500; // Now the game doesn't trigger the use item twice
using tInvRemove = unsigned long long(__fastcall*)(long long, int);
static tInvRemove oInvRemove = nullptr;

static ULONGLONG lastT = 0;
static bool g_ItemGateArmed = true;

static unsigned long long __fastcall HookedInvRemove(long long a1, int a2) {
  if (g_ItemGateArmed)
  {
    ULONGLONG now = GetTickCount64();
    if (now - lastT < ITEM_GATE_MS) return 0;
    lastT = now;
    return oInvRemove(a1, a2);
  }
  return oInvRemove(a1, a2);
}

bool ItemHook() {
  uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
  void* t = (void*)(base + INV_REMOVE_RVA);
  if (MH_CreateHook(t, reinterpret_cast<LPVOID>(&HookedInvRemove),
      reinterpret_cast<LPVOID*>(&oInvRemove)) != MH_OK) return false;
  return MH_EnableHook(t) == MH_OK;
}

static HRESULT STDMETHODCALLTYPE HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    return oPresent(pSwapChain, SyncInterval, Flags);
}

bool PresentHook()
{

    if (!GetPresentAddress(&presentAddr)) 
        return false;

    if (MH_CreateHook(presentAddr, reinterpret_cast<LPVOID>(&HookedPresent), reinterpret_cast<LPVOID*>(&oPresent)) != MH_OK)
        return false;

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        return false;

    return true;
}

void PresentUnhook()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    oPresent = nullptr;
}