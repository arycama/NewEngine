#pragma once

#include "GraphicsDevice.h"

#include <memory>
#include <wrl/client.h>

class D3D11GraphicsContext;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct IDXGISwapChain;

class D3D11GraphicsDevice : public GraphicsDevice
{
public:
	D3D11GraphicsDevice(int width, int height, bool vsync, HWND hwnd, bool fullscreen);
	~D3D11GraphicsDevice();

	void BeginScene(float red, float green, float blue, float alpha) const override;
	void EndScene() const override;

	GraphicsContext& GetGraphicsContext() const override;

	float GetAspectRatio() const override;
	void CreateTexture2D(int width, int height, ID3D11Texture2D** texture) override;
	void CreateShaderResourceView(ID3D11Resource& resource, TextureFormat format, ID3D11ShaderResourceView** result) override;

	void CreateVertexShader(const void* shaderBytecode, int size, struct ID3D11VertexShader** vertexShader) override;

	void CreateInputLayout(const struct D3D11_INPUT_ELEMENT_DESC* inputs, int count, void* shader, int shaderSize, struct ID3D11InputLayout** result) override;

	void CreatePixelShader(const void* bytecode, int size, struct ID3D11PixelShader** result) override;

	void CreateSamplerState(struct CD3D11_SAMPLER_DESC& desc, struct ID3D11SamplerState** result) override;

	void CreateBuffer(const struct CD3D11_BUFFER_DESC& desc, const struct D3D11_SUBRESOURCE_DATA& initialData, struct ID3D11Buffer** result) override;

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

	std::unique_ptr<D3D11GraphicsContext> context;
};
