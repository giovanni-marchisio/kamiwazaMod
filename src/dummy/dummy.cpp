#include "dummy.h"
#include <windows.h>
#include <d3d11.h>

constexpr UINT kPresentIndex = 8;
constexpr UINT kResizeBuffersIndex = 13;

static bool GetSwapChainVFunction(UINT index, void** outFunc)
{
	if (!outFunc)
		return false;

	HWND hwnd = GetForegroundWindow();
	if (!hwnd)
		hwnd = GetDesktopWindow();

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	IDXGISwapChain* swapchain = nullptr;

	if (D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swapchain,
		&device,
		nullptr,
		&context) != S_OK)
	{
		return false;
	}

	void** vtable = *reinterpret_cast<void***>(swapchain);
	*outFunc = vtable[index];

	if (context) context->Release();
	if (device) device->Release();
	if (swapchain) swapchain->Release();

	return true;
}

bool GetPresentAddress(void** outPresent)
{
	return GetSwapChainVFunction(kPresentIndex, outPresent);
}

bool GetResizeBuffersAddress(void** outResize)
{
	return GetSwapChainVFunction(kResizeBuffersIndex, outResize);
}
