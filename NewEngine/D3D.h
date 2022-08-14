#pragma once

// Linking
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

class D3D
{
public:
	D3D(int, int, bool, HWND, bool, float, float);
	~D3D();

	D3DXMATRIX GetProjectionMatrix();
	D3DXMATRIX GetWorldMatrix();
	D3DXMATRIX GetOrthoMatrix();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void BeginScene(D3DXCOLOR);
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
	D3DXMATRIX projectionMatrix, worldMatrix, orthoMatrix;
};
