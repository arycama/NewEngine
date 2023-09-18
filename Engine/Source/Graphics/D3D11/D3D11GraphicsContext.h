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

private:
	ID3D11DeviceContext& deviceContext;
};