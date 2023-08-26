#pragma once

#include <wrl/client.h>

struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct IDXGISwapChain;

class Graphics
{
public:
	Graphics(int width, int height, bool vsync, HWND hwnd, bool fullscreen);
	~Graphics();

	void BeginScene(float red, float green, float blue, float alpha) const;
	void EndScene() const;

	ID3D11Device& GetDevice() const;
	ID3D11DeviceContext& GetDeviceContext() const;

	float GetAspectRatio() const;

private:
	const int width, height;
	const bool vsync;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
};
