#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <directxmath.h>
#include <wrl/client.h>

class Graphics
{
public:
	Graphics(UINT, UINT, bool, HWND, bool);
	~Graphics();

	void BeginScene(const FLOAT red, const FLOAT green, const FLOAT blue, const FLOAT alpha) const;
	void EndScene() const;

	struct ID3D11Device& GetDevice() const;
	struct ID3D11DeviceContext& GetDeviceContext() const;

	float GetAspectRatio() const;

private:
	int width, height;
	bool vsyncEnabled;

	Microsoft::WRL::ComPtr<struct IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<struct ID3D11Device> device;
	Microsoft::WRL::ComPtr<struct ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<struct ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<struct ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<struct ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<struct ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<struct ID3D11RasterizerState> rasterState;

	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;
};
