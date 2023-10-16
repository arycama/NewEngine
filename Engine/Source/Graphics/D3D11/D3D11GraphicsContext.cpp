#include "D3D11GraphicsContext.h"
#include "Handle.h"

#include <d3d11.h>
#include <comdef.h>

using namespace _com_util;

D3D11GraphicsContext::D3D11GraphicsContext(ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext) { }

D3D11GraphicsContext::~D3D11GraphicsContext()
{
}

void D3D11GraphicsContext::BeginWrite(const Handle& handle, D3D11_MAPPED_SUBRESOURCE* mappedResource)
{
	auto& buffer = buffers.GetResource(handle);
	CheckError(deviceContext.Map(&buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, mappedResource));
}

void D3D11GraphicsContext::EndWrite(const Handle& handle)
{
	auto& buffer = buffers.GetResource(handle);
	deviceContext.Unmap(&buffer, 0);
}

void D3D11GraphicsContext::VSSetConstantBuffers(int start, int count, const Handle& handle)
{
	auto resource = &buffers.GetResource(handle);
	deviceContext.VSSetConstantBuffers(start, count, &resource);
}

void D3D11GraphicsContext::PSSetShaderResources(int start, int count, const Handle& handle)
{
	auto shaderResourceView = &shaderResourceViews.GetResource(handle);
	deviceContext.PSSetShaderResources(start, count, &shaderResourceView);
}

void D3D11GraphicsContext::PSSetSamplers(int start, int count, const Handle& handle)
{
	auto handles = &samplerStates.GetResource(handle);
	deviceContext.PSSetSamplers(start, count, &handles);
}

void D3D11GraphicsContext::IASetInputLayout(const Handle& handle)
{
	auto& layout = inputLayouts.GetResource(handle);
	deviceContext.IASetInputLayout(&layout);
}

void D3D11GraphicsContext::VSSetShader(const Handle& handle)
{
	auto& shader = vertexShaders.GetResource(handle);
	deviceContext.VSSetShader(&shader, nullptr, 0);
}

void D3D11GraphicsContext::PSSetShader(const Handle& handle)
{
	auto& shader = pixelShaders.GetResource(handle);
	deviceContext.PSSetShader(&shader, nullptr, 0);
}

void D3D11GraphicsContext::IASetVertexBuffers(int start, int count, const Handle& handle, int stride, int offset)
{
	auto stride1 = static_cast<UINT>(stride);
	auto offset1 = static_cast<UINT>(offset);
	auto buffer = &buffers.GetResource(handle);
	deviceContext.IASetVertexBuffers(start, count, &buffer, &stride1, &offset1);
}

void D3D11GraphicsContext::IASetIndexBuffer(const Handle& handle)
{
	deviceContext.IASetIndexBuffer(&buffers.GetResource(handle), DXGI_FORMAT_R32_UINT, 0);
}

void D3D11GraphicsContext::IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	deviceContext.IASetPrimitiveTopology(topology);
}

void D3D11GraphicsContext::DrawIndexed(int count, int indexStart, int vertexStart)
{
	deviceContext.DrawIndexed(count, indexStart, vertexStart);
}

void D3D11GraphicsContext::UpdateTextureSubresource(const Handle& handle, int subresource, D3D11_BOX* box, void* data, int rowPitch, int depthPitch)
{
	auto& resource = textures.GetResource(handle);
	deviceContext.UpdateSubresource(&resource, subresource, box, data, rowPitch, depthPitch);
}

void D3D11GraphicsContext::GenerateMips(const Handle& handle)
{
	auto& shaderResourceView = shaderResourceViews.GetResource(handle);
	deviceContext.GenerateMips(&shaderResourceView);
}
