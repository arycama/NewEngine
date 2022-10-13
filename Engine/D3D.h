#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <memory>
#include <d3d11.h>
#include <directxmath.h>

class D3D
{
public:
	D3D(int, int, bool, HWND, bool, float, float);
	~D3D();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	DirectX::XMMATRIX& GetProjectionMatrix();
	DirectX::XMMATRIX& GetWorldMatrix();
	DirectX::XMMATRIX& GetOrthoMatrix();

	void GetVideoCardInfo(char*, int&);

private:
	bool vsyncEnabled;
	int videoCardMemory;
	char videoCardDescription[128];

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	std::unique_ptr<ID3D11Texture2D> depthStencilBuffer;
	std::unique_ptr<ID3D11DepthStencilState> depthStencilState;
	std::unique_ptr<ID3D11DepthStencilView> depthStencilView;
	std::unique_ptr<ID3D11RasterizerState> rasterState;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;
};
