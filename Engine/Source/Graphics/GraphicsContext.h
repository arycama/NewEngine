#pragma once

enum D3D_PRIMITIVE_TOPOLOGY;

class GraphicsContext
{
public:
	virtual void BeginWrite(struct ID3D11Resource* resource, struct D3D11_MAPPED_SUBRESOURCE* mappedResource) = 0;
	virtual void EndWrite(ID3D11Resource* resource) = 0;

	// Todo: General Shader.SetConstantBuffer function with shader stage as enum?
	// Todo: Make these accept vectors or something
	virtual void VSSetConstantBuffers(int start, int count, struct ID3D11Buffer* const* constantBuffers) = 0;
	virtual void PSSetShaderResources(int start, int count, struct ID3D11ShaderResourceView* const* ppShaderResourceViews) = 0;
	virtual void PSSetSamplers(int start, int count, struct ID3D11SamplerState* const* ppSamplers) = 0;
	virtual void IASetInputLayout(struct ID3D11InputLayout& layout) = 0;
	virtual void VSSetShader(struct ID3D11VertexShader& shader) = 0;
	virtual void PSSetShader(struct ID3D11PixelShader& shader) = 0;
	virtual void IASetVertexBuffers(int start, int count, ID3D11Buffer* const* vertexBuffer, int stride, int offset) = 0;
	virtual void IASetIndexBuffer(ID3D11Buffer* buffer) = 0;
	virtual void IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) = 0;
	virtual void DrawIndexed(int count, int indexStart, int vertexStart) = 0;
};