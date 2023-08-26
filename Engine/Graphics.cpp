#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Graphics.h"
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

Graphics::Graphics(int width, int height, bool vsync, HWND hwnd, bool fullscreen) : width(width), height(height), vsync(vsync)
{
	// Create a DirectX graphics interface factory.
	ComPtr<IDXGIFactory> factory;
	CheckError(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf()));

	// Use the factory to create an adapter for the primary graphics interface (video card).
	ComPtr<IDXGIAdapter> adapter;
	CheckError(factory->EnumAdapters(0, adapter.GetAddressOf()));

	// Enumerate the primary adapter output (monitor).
	ComPtr<IDXGIOutput> adapterOutput;
	CheckError(adapter->EnumOutputs(0, adapterOutput.GetAddressOf()));

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	UINT numModes = 0;
	CheckError(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr));

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	auto displayModeList = unique_ptr<DXGI_MODE_DESC[]>(new DXGI_MODE_DESC[numModes]);

	// Now fill the display mode list structures.
	CheckError(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.get()));

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	int numerator, denominator;
	for (auto i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == width)
		{
			if (displayModeList[i].Height == height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	CheckError(adapter->GetDesc(&adapterDesc));

	auto refreshRate = DXGI_RATIONAL{ static_cast<UINT>(vsync ? numerator : 0), static_cast<UINT>(vsync ? denominator : 1) };
	auto bufferDesc = DXGI_MODE_DESC{ static_cast<UINT>(width), static_cast<UINT>(height), refreshRate, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED };
	auto sampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC{ bufferDesc, sampleDesc, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1, hwnd, !fullscreen, DXGI_SWAP_EFFECT_DISCARD, 0 };

	// Set the feature level to DirectX 11.
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	CheckError(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(), device.GetAddressOf(), nullptr, deviceContext.GetAddressOf()));

	// Get the pointer to the back buffer.
	ComPtr<ID3D11Texture2D> backBufferPtr;
	CheckError(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf()));

	// Create the render target view with the back buffer pointer.
	CheckError(device->CreateRenderTargetView(backBufferPtr.Get(), nullptr, renderTargetView.GetAddressOf()));

	// Initialize the description of the depth buffer.
	auto depthBufferDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	// Create the texture for the depth buffer using the filled out description.
	CheckError(device->CreateTexture2D(&depthBufferDesc, nullptr, depthStencilBuffer.GetAddressOf()));

	// Initialize the description of the stencil state.
	auto depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC(true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false, 0xFF, 0xFF, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);

	// Create the depth stencil state.
	CheckError(device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf()));

	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	// Set up the depth stencil view description.
	auto depthStencilViewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);

	// Create the depth stencil view.
	CheckError(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()));

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	// Setup the raster description which will determine how and what polygons will be drawn.
	auto rasterDesc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_NONE, false, 0, 0.0f, 0.0f, true, false, false, false);

	// Create the rasterizer state from the description we just filled out.
	CheckError(device->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf()));

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterState.Get());

	// Setup the viewport for rendering.
	auto viewport = D3D11_VIEWPORT{ 0.0f, 0.0f, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);
}

Graphics::~Graphics()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	swapChain->SetFullscreenState(false, nullptr);
}

void Graphics::BeginScene(float red, float green, float blue, float alpha) const
{
	// Setup the color to clear the buffer to.
	const float color[4]{ red, green, blue, alpha };

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::EndScene() const
{
	// Lock to screen refresh rate.
	swapChain->Present(vsync ? 1 : 0, 0);
}

ID3D11Device& Graphics::GetDevice() const
{
	return *device.Get();
}

ID3D11DeviceContext& Graphics::GetDeviceContext() const
{
	return *deviceContext.Get();
}

float Graphics::GetAspectRatio() const
{
	return static_cast<float>(width) / static_cast<float>(height);
}
