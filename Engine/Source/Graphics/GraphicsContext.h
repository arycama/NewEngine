#pragma once

class GraphicsContext
{
public:
	virtual void BeginWrite(struct ID3D11Resource* resource, struct D3D11_MAPPED_SUBRESOURCE* mappedResource) = 0;
	virtual void EndWrite(ID3D11Resource* resource) = 0;

	// Todo: General Shader.SetConstantBuffer function with shader stage as enum?
	virtual void VSSetConstantBuffers(int start, int count, struct ID3D11Buffer* const* constantBuffers) = 0;
};