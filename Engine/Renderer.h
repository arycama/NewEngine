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

class Renderer
{
public:
	Renderer(UINT, UINT, bool, HWND, bool);
	~Renderer();

	void BeginScene(const FLOAT red, const FLOAT green, const FLOAT blue, const FLOAT alpha) const;
	void EndScene() const;

	ID3D11Device& GetDevice() const;
	ID3D11DeviceContext& GetDeviceContext() const;

	float GetAspectRatio() const;

private:
	int width, height;
	bool vsyncEnabled;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;

	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;
};
