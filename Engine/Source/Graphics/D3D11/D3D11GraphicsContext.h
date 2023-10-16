#pragma once

#include "GraphicsContext.h"
#include "HandlePool.h"

#include <queue>
#include <vector>

class Handle;
struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

class D3D11GraphicsContext : public GraphicsContext
{
public:
	D3D11GraphicsContext(ID3D11DeviceContext& deviceContext);
	~D3D11GraphicsContext();

	void BeginWrite(const Handle& handle, struct D3D11_MAPPED_SUBRESOURCE* mappedResource) override;
	void EndWrite(const Handle& handle) override;

	void VSSetConstantBuffers(int start, int count, const Handle& handle) override;
	void PSSetShaderResources(int start, int count, const Handle& handle) override;
	void PSSetSamplers(int start, int count, const Handle& handle) override;
	void IASetInputLayout(const Handle& handle) override;
	void VSSetShader(const Handle& handle) override;
	void PSSetShader(const Handle& handle) override;
	void IASetVertexBuffers(int start, int count, const Handle& vertexBuffer, int stride, int offset) override;
	void IASetIndexBuffer(const Handle& buffer) override;
	void IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) override;
	void DrawIndexed(int count, int indexStart, int vertexStart) override;
	void UpdateTextureSubresource(const Handle& texure, int subresource, struct D3D11_BOX* box, void* data, int rowPitch, int depthPitch) override;
	void GenerateMips(const Handle& shaderResourceView) override;

	HandlePool<ID3D11Buffer> buffers;
	HandlePool<ID3D11SamplerState> samplerStates;
	HandlePool<ID3D11VertexShader> vertexShaders;
	HandlePool<ID3D11PixelShader> pixelShaders;
	HandlePool<ID3D11InputLayout> inputLayouts;
	HandlePool<ID3D11Texture2D> textures;
	HandlePool<ID3D11ShaderResourceView> shaderResourceViews;

private:
	ID3D11DeviceContext& deviceContext;
};