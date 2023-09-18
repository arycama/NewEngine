#include "D3D11GraphicsContext.h"

#include <d3d11.h>
#include <comdef.h>

using namespace _com_util;

D3D11GraphicsContext::D3D11GraphicsContext(ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext) { }

void D3D11GraphicsContext::BeginWrite(ID3D11Resource* resource, D3D11_MAPPED_SUBRESOURCE* mappedResource)
{
	CheckError(deviceContext.Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, mappedResource));
}

void D3D11GraphicsContext::EndWrite(ID3D11Resource* resource)
{
	deviceContext.Unmap(resource, 0);
}

void D3D11GraphicsContext::VSSetConstantBuffers(int start, int count, ID3D11Buffer* const* constantBuffers)
{
	deviceContext.VSSetConstantBuffers(start, count, constantBuffers);
}
