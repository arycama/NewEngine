#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "D3D11GraphicsDevice.h"
#include "D3D11GraphicsContext.h"

#include <d3d11.h>
#include <memory>
#include <dxgi.h>
#include <dxgicommon.h>
#include <dxgiformat.h>
#include <dxgitype.h>
#include <minwindef.h>
#include <windef.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <comdef.h>
#include <new>

using namespace std;
using namespace Microsoft::WRL;
using namespace _com_util;

D3D11GraphicsDevice::D3D11GraphicsDevice(int width, int height, bool vsync, HWND hwnd, bool fullscreen) : width(width), height(height), vsync(vsync)
{
	ComPtr<IDXGIFactory> factory;
	CheckError(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf()));

	ComPtr<IDXGIAdapter> adapter;
	CheckError(factory->EnumAdapters(0, adapter.GetAddressOf()));

	ComPtr<IDXGIOutput> adapterOutput;
	CheckError(adapter->EnumOutputs(0, adapterOutput.GetAddressOf()));

	UINT numModes;
	CheckError(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr));

	auto displayModes = make_unique<DXGI_MODE_DESC[]>(numModes);
	CheckError(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes.get()));

	// Find the display mode that matches our resolution
	int numerator = 0, denominator = 0;
	for (auto i = 0; i < numModes; i++)
	{
		auto& displayMode = displayModes[i];
		if (displayMode.Width != width || displayMode.Height != height)
			continue;

		numerator = displayMode.RefreshRate.Numerator;
		denominator = displayMode.RefreshRate.Denominator;
		break;
	}

	DXGI_RATIONAL refreshRate{ static_cast<UINT>(vsync ? numerator : 0), static_cast<UINT>(vsync ? denominator : 1) };
	DXGI_MODE_DESC bufferDesc{ static_cast<UINT>(width), static_cast<UINT>(height), refreshRate, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED };
	DXGI_SAMPLE_DESC sampleDesc{ 1, 0 };
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ bufferDesc, sampleDesc, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1, hwnd, !fullscreen, DXGI_SWAP_EFFECT_DISCARD, 0 };

	auto featureLevel = D3D_FEATURE_LEVEL_11_0;
	CheckError(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(), device.GetAddressOf(), nullptr, deviceContext.GetAddressOf()));

	ComPtr<ID3D11Texture2D> backBufferPtr;
	CheckError(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBufferPtr.GetAddressOf()));
	CheckError(device->CreateRenderTargetView(backBufferPtr.Get(), nullptr, renderTargetView.GetAddressOf()));

	CD3D11_TEXTURE2D_DESC depthBufferDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	CheckError(device->CreateTexture2D(&depthBufferDesc, nullptr, depthStencilBuffer.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false, 0xFF, 0xFF, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);
	CheckError(device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
	CheckError(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()));

	CD3D11_RASTERIZER_DESC rasterDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE, false, 0, 0.0f, 0.0f, true, false, false, false);
	CheckError(device->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf()));

	// These are just set once as they never change, but as more render passes are set, they may need to be configured differently
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	D3D11_VIEWPORT viewport{ 0.0f, 0.0f, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);

	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	deviceContext->RSSetState(rasterState.Get());

	context = make_unique<D3D11GraphicsContext>(*deviceContext.Get());
}

D3D11GraphicsDevice::~D3D11GraphicsDevice()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	swapChain->SetFullscreenState(false, nullptr);
}

void D3D11GraphicsDevice::BeginScene(float red, float green, float blue, float alpha) const
{
	const float color[4]{ red, green, blue, alpha };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D11GraphicsDevice::EndScene() const
{
	// Lock to screen refresh rate.
	swapChain->Present(vsync ? 1 : 0, 0);
}

ID3D11Device& D3D11GraphicsDevice::GetDevice() const
{
	return *device.Get();
}

GraphicsContext& D3D11GraphicsDevice::GetGraphicsContext() const
{
	return *context.get();
}

float D3D11GraphicsDevice::GetAspectRatio() const
{
	return static_cast<float>(width) / static_cast<float>(height);
}
