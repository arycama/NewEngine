#pragma once

#include "GraphicsContext.h"

struct ID3D11DeviceContext;

class D3D11GraphicsContext : public GraphicsContext
{
public:
	D3D11GraphicsContext(ID3D11DeviceContext& deviceContext);

	void BeginWrite(struct ID3D11Resource* resource, struct D3D11_MAPPED_SUBRESOURCE* mappedResource) override;
	void EndWrite(ID3D11Resource* resource) override;

	void VSSetConstantBuffers(int start, int count, struct ID3D11Buffer* const* constantBuffers) override;
	void PSSetShaderResources(int start, int count, struct ID3D11ShaderResourceView* const* ppShaderResourceViews) override;
	void PSSetSamplers(int start, int count, struct ID3D11SamplerState* const* ppSamplers) override;
	void IASetInputLayout(ID3D11InputLayout& layout) override;
	void VSSetShader(ID3D11VertexShader& shader) override;
	void PSSetShader(ID3D11PixelShader& shader) override;
	void IASetVertexBuffers(int start, int count, ID3D11Buffer* const* vertexBuffer, int stride, int offset) override;
	void IASetIndexBuffer(ID3D11Buffer* buffer) override;
	void IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) override;
	void DrawIndexed(int count, int indexStart, int vertexStart) override;

private:
	ID3D11DeviceContext& deviceContext;
};