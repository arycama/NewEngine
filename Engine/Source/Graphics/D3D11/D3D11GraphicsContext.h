#pragma once

#include "GraphicsContext.h"

#include <queue>
#include <vector>

class Handle;
struct ID3D11DeviceContext;

class D3D11GraphicsContext : public GraphicsContext
{
public:
	D3D11GraphicsContext(ID3D11DeviceContext& deviceContext);
	~D3D11GraphicsContext();

	void BeginWrite(const Handle& handle, struct D3D11_MAPPED_SUBRESOURCE* mappedResource) override;
	void EndWrite(const Handle& handle) override;

	void VSSetConstantBuffers(int start, int count, const Handle& handle) override;
	void PSSetShaderResources(int start, int count, struct ID3D11ShaderResourceView* const* ppShaderResourceViews) override;
	void PSSetSamplers(int start, int count, struct ID3D11SamplerState* const* ppSamplers) override;
	void IASetInputLayout(ID3D11InputLayout& layout) override;
	void VSSetShader(ID3D11VertexShader& shader) override;
	void PSSetShader(ID3D11PixelShader& shader) override;
	void IASetVertexBuffers(int start, int count, const Handle& vertexBuffer, int stride, int offset) override;
	void IASetIndexBuffer(const Handle& buffer) override;
	void IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) override;
	void DrawIndexed(int count, int indexStart, int vertexStart) override;
	void UpdateSubresource(ID3D11Resource& resource, int subresource, struct D3D11_BOX* box, void* data, int rowPitch, int depthPitch) override;
	void GenerateMips(ID3D11ShaderResourceView& shaderResourceView) override;

	std::vector<std::pair<struct ID3D11Buffer*, int>> buffers;
	std::queue<std::pair<int, int>> availableIndices;

private:
	ID3D11DeviceContext& deviceContext;
};