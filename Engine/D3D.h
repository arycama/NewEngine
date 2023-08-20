#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <directxmath.h>
#include <wrl/client.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;

class D3D
{
public:
	D3D(UINT, UINT, bool, HWND, bool);
	~D3D();

	void BeginScene(const FLOAT red, const FLOAT green, const FLOAT blue, const FLOAT alpha) const;
	void EndScene() const;

	ID3D11Device& GetDevice() const;
	ID3D11DeviceContext& GetDeviceContext() const;

	DirectX::XMMATRIX& GetProjectionMatrix();
	DirectX::XMMATRIX& GetWorldMatrix();
	DirectX::XMMATRIX& GetOrthoMatrix();

private:
	bool vsyncEnabled;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;
};
