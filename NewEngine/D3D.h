#pragma once

// Linking
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <DXGI.h>
#include <D3DCommon.h>
#include <D3D11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

class D3D
{
public:
	D3D(int, int, bool, HWND, bool, float, float);
	~D3D();

	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void BeginScene(DirectX::PackedVector::XMCOLOR);
	void EndScene();
	void GetVideoCardInfo(char*, int&);

private:
	bool vsyncEnabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	DirectX::XMMATRIX projectionMatrix, worldMatrix, orthoMatrix;
};
