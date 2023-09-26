#include "D3D11GraphicsBuffer.h"

#include <d3d11.h>
#include <comdef.h>

using namespace _com_util;

D3D11GraphicsBuffer::D3D11GraphicsBuffer(ID3D11Device& device, ID3D11DeviceContext& context, int size, D3D11_BIND_FLAG type, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG access, const D3D11_SUBRESOURCE_DATA* initialData) : context(context)
{
	CD3D11_BUFFER_DESC desc(size, static_cast<D3D11_BIND_FLAG>(type), static_cast<D3D11_USAGE>(usage), static_cast<D3D11_CPU_ACCESS_FLAG>(access));
	CheckError(device.CreateBuffer(&desc, initialData, data.GetAddressOf()));
}

void* D3D11GraphicsBuffer::BeginWrite()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CheckError(context.Map(data.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	return mappedResource.pData;
}

void D3D11GraphicsBuffer::EndWrite()
{
}
