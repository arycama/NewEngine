#include "D3D11GraphicsContext.h"
#include "Handle.h"

#include <d3d11.h>
#include <comdef.h>

using namespace _com_util;

D3D11GraphicsContext::D3D11GraphicsContext(ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext) { }

D3D11GraphicsContext::~D3D11GraphicsContext()
{
	for (auto& buffer : buffers)
		buffer.first->Release();

}

void D3D11GraphicsContext::BeginWrite(const Handle& handle, D3D11_MAPPED_SUBRESOURCE* mappedResource)
{
	auto& buffer = buffers.at(handle.GetIndex());
	CheckError(deviceContext.Map(buffer.first, 0, D3D11_MAP_WRITE_DISCARD, 0, mappedResource));
}

void D3D11GraphicsContext::EndWrite(const Handle& handle)
{
	auto& buffer = buffers.at(handle.GetIndex()).first;
	deviceContext.Unmap(buffer, 0);
}

void D3D11GraphicsContext::VSSetConstantBuffers(int start, int count, const Handle& handle)
{
	auto& buffer = buffers.at(handle.GetIndex()).first;
	deviceContext.VSSetConstantBuffers(start, count, &buffer);
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

void D3D11GraphicsContext::IASetVertexBuffers(int start, int count, const Handle& handle, int stride, int offset)
{
	auto stride1 = static_cast<UINT>(stride);
	auto offset1 = static_cast<UINT>(offset);
	auto& buffer = buffers.at(handle.GetIndex());
	deviceContext.IASetVertexBuffers(start, count, &buffer.first, &stride1, &offset1);
}

void D3D11GraphicsContext::IASetIndexBuffer(const Handle& handle)
{
	deviceContext.IASetIndexBuffer(buffers.at(handle.GetIndex()).first, DXGI_FORMAT_R32_UINT, 0);
}

void D3D11GraphicsContext::IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	deviceContext.IASetPrimitiveTopology(topology);
}

void D3D11GraphicsContext::DrawIndexed(int count, int indexStart, int vertexStart)
{
	deviceContext.DrawIndexed(count, indexStart, vertexStart);
}

void D3D11GraphicsContext::UpdateSubresource(ID3D11Resource& resource, int subresource, D3D11_BOX* box, void* data, int rowPitch, int depthPitch)
{
	deviceContext.UpdateSubresource(&resource, subresource, box, data, rowPitch, depthPitch);
}

void D3D11GraphicsContext::GenerateMips(ID3D11ShaderResourceView& shaderResourceView)
{
	deviceContext.GenerateMips(&shaderResourceView);
}
