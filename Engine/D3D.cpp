#include "D3D.h"

#include <comdef.h>
#include <memory>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace _com_util;

const float screenDepth = 1000.0f;
const float screenNear = 0.1f;

D3D::D3D(UINT width, UINT height, bool vsync, HWND hwnd, bool fullscreen)
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
	UINT numerator, denominator;
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

	auto refreshRate = DXGI_RATIONAL{ vsync ? numerator : 0, vsync ? denominator : 1 };
	auto bufferDesc = DXGI_MODE_DESC{ width, height, refreshRate, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED };
	auto sampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC{ bufferDesc, sampleDesc, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1, hwnd, !fullscreen, DXGI_SWAP_EFFECT_DISCARD, 0 };

	// Set the feature level to DirectX 11.
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	CheckError(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(), &device, nullptr, &deviceContext));

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
	auto depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC(true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_ALWAYS, true, 0xFF, 0xFF, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);

	// Create the depth stencil state.
	CheckError(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	CheckError(device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, &depthStencilView));

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	// Setup the raster description which will determine how and what polygons will be drawn.
	auto rasterDesc = D3D11_RASTERIZER_DESC{ D3D11_FILL_SOLID, D3D11_CULL_BACK, false, 0, 0.0f, 0.0f, true, false, false, false };

	// Create the rasterizer state from the description we just filled out.
	CheckError(device->CreateRasterizerState(&rasterDesc, &rasterState));

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterState.Get());

	// Setup the viewport for rendering.
	auto viewport = D3D11_VIEWPORT{ 0.0f, 0.0f, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	auto screenAspect = static_cast<float>(width) / static_cast<float>(height);

	// Create the projection matrix for 3D rendering.
	projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(width), static_cast<float>(height), screenNear, screenDepth);
}

D3D::~D3D()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	swapChain->SetFullscreenState(false, nullptr);
}

void D3D::BeginScene(const FLOAT red, const FLOAT green, const FLOAT blue, const FLOAT alpha) const
{
	// Setup the color to clear the buffer to.
	const FLOAT color[4]{ red, green, blue, alpha };

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D::EndScene() const
{
	// Lock to screen refresh rate.
	swapChain->Present(vsyncEnabled ? 1 : 0, 0);
}

ID3D11Device* D3D::GetDevice() const
{
	return device.Get();
}

ID3D11DeviceContext* D3D::GetDeviceContext() const
{
	return deviceContext.Get();
}

XMMATRIX& D3D::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMMATRIX& D3D::GetWorldMatrix()
{
	return worldMatrix;
}

XMMATRIX& D3D::GetOrthoMatrix()
{
	return orthoMatrix;
}
