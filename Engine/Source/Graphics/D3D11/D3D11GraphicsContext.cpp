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

void D3D11GraphicsContext::PSSetShaderResources(int start, int count, ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	deviceContext.PSSetShaderResources(start, count, ppShaderResourceViews);
}

void D3D11GraphicsContext::PSSetSamplers(int start, int count, ID3D11SamplerState* const* ppSamplers)
{
	deviceContext.PSSetSamplers(start, count, ppSamplers);
}

void D3D11GraphicsContext::IASetInputLayout(ID3D11InputLayout& layout)
{
	deviceContext.IASetInputLayout(&layout);
}

void D3D11GraphicsContext::VSSetShader(ID3D11VertexShader& shader)
{
	deviceContext.VSSetShader(&shader, nullptr, 0);
}

void D3D11GraphicsContext::PSSetShader(ID3D11PixelShader& shader)
{
	deviceContext.PSSetShader(&shader, nullptr, 0);
}

void D3D11GraphicsContext::IASetVertexBuffers(int start, int count, ID3D11Buffer* const* vertexBuffer, int stride, int offset)
{
	auto stride1 = static_cast<UINT>(stride);
	auto offset1 = static_cast<UINT>(offset);
	deviceContext.IASetVertexBuffers(start, count, vertexBuffer, &stride1, &offset1);
}

void D3D11GraphicsContext::IASetIndexBuffer(ID3D11Buffer* buffer)
{
	deviceContext.IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}

void D3D11GraphicsContext::IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	deviceContext.IASetPrimitiveTopology(topology);
}

void D3D11GraphicsContext::DrawIndexed(int count, int indexStart, int vertexStart)
{
	deviceContext.DrawIndexed(count, indexStart, vertexStart);
}
