#pragma once

class Handle;
enum D3D_PRIMITIVE_TOPOLOGY;

class GraphicsContext
{
public:
	virtual void BeginWrite(const Handle& handle, struct D3D11_MAPPED_SUBRESOURCE* mappedResource) = 0;
	virtual void EndWrite(const Handle& handle) = 0;

	// Todo: General Shader.SetConstantBuffer function with shader stage as enum?
	// Todo: Make these accept vectors or something
	virtual void VSSetConstantBuffers(int start, int count, const Handle& handle) = 0;
	virtual void PSSetShaderResources(int start, int count, const Handle& handle) = 0;
	virtual void PSSetSamplers(int start, int count, const Handle& handle) = 0;
	virtual void IASetInputLayout(const Handle& handle) = 0;
	virtual void VSSetShader(const Handle& handle) = 0;
	virtual void PSSetShader(const Handle& handle) = 0;
	virtual void IASetVertexBuffers(int start, int count, const Handle& vertexBuffer, int stride, int offset) = 0;
	virtual void IASetIndexBuffer(const Handle& handle) = 0;
	virtual void IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) = 0;
	virtual void DrawIndexed(int count, int indexStart, int vertexStart) = 0;
	virtual void UpdateTextureSubresource(const Handle& handle, int subresource, struct D3D11_BOX* box, void* data, int rowPitch, int depthPitch) = 0;
	virtual void GenerateMips(const Handle& handle) = 0;
};