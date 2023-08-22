#include "Shader.h"

#include <comdef.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include <memory>

using namespace _com_util;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

Shader::Shader(ID3D11Device& device)
{
	// Create the vertex shader
	ComPtr<ID3D10Blob> vertexShaderBuffer;
	CheckError(D3DCompileFromFile(L"../Engine/Texture.hlsl", nullptr, nullptr, "Vertex", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), nullptr));
	CheckError(device.CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// Create the pixel shader
	ComPtr<ID3D10Blob> pixelShaderBuffer;
	CheckError(D3DCompileFromFile(L"../Engine/Texture.hlsl", nullptr, nullptr, "Pixel", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), nullptr));
	CheckError(device.CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the Model and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Create the vertex input layout.
	CheckError(device.CreateInputLayout(polygonLayout, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout));

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	auto matrixBufferDesc = CD3D11_BUFFER_DESC(sizeof(MatrixBufferType), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer));

	// Create a texture sampler state description.
	// Create the texture sampler state.
	FLOAT borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	auto samplerDesc = CD3D11_SAMPLER_DESC(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, borderColor, 0, D3D11_FLOAT32_MAX);
	CheckError(device.CreateSamplerState(&samplerDesc, &samplerState));
}

void Shader::Render(ID3D11DeviceContext& deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView& texture) const
{
	// Set the shader parameters that it will use for rendering.
	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CheckError(deviceContext.Map(matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// Get a pointer to the data in the constant buffer.
	auto dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext.Unmap(matrixBuffer.Get(), 0);

	// Set the vertex input layout.
	deviceContext.IASetInputLayout(layout.Get());
	deviceContext.VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext.VSSetConstantBuffers(0, 1, matrixBuffer.GetAddressOf());

	// Set shader texture resource in the pixel shader.
	deviceContext.PSSetShader(pixelShader.Get(), nullptr, 0);
	auto shaderResourceViews = &texture;
	deviceContext.PSSetShaderResources(0, 1, &shaderResourceViews);
	deviceContext.PSSetSamplers(0, 1, samplerState.GetAddressOf());
}
