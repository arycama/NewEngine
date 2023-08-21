#include "Shader.h"

#include <comdef.h>
#include <memory>

using namespace _com_util;

using namespace std;
using namespace DirectX;

Shader::Shader(ID3D11Device& device)
{
	wchar_t vsFilename[128];
	wchar_t psFilename[128];

	// Set the filename of the vertex shader.
	int error = wcscpy_s(vsFilename, 128, L"../Engine/Texture.hlsl");
	if (error != 0)
	{
		//return false;
	}

	// Set the filename of the pixel shader.
	error = wcscpy_s(psFilename, 128, L"../Engine/Texture.hlsl");
	if (error != 0)
	{
		//return false;
	}

	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "Vertex", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	CheckError(result);

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, "Pixel", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	CheckError(result);

	// Create the vertex shader from the buffer.
	result = device.CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	CheckError(result);

	// Create the pixel shader from the buffer.
	result = device.CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	CheckError(result);

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the Model and in the shader.
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

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device.CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);
	CheckError(result);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device.CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	CheckError(result);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device.CreateSamplerState(&samplerDesc, &sampleState);
	if (FAILED(result))
	{
		return;
	}
}

bool Shader::Render(ID3D11DeviceContext& deviceContext, int indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView& texture)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool Shader::SetShaderParameters(ID3D11DeviceContext& deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView& texture) const
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = deviceContext.Map(matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext.Unmap(matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext.VSSetConstantBuffers(bufferNumber, 1, matrixBuffer.GetAddressOf());

	ID3D11ShaderResourceView* test = &texture;

	// Set shader texture resource in the pixel shader.
	deviceContext.PSSetShaderResources(0, 1, &(test));

	return true;
}

void Shader::RenderShader(ID3D11DeviceContext& deviceContext, int indexCount) const
{
	// Set the vertex input layout.
	deviceContext.IASetInputLayout(layout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext.VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext.PSSetShader(pixelShader.Get(), nullptr, 0);

	// Set the sampler state in the pixel shader.
	deviceContext.PSSetSamplers(0, 1, sampleState.GetAddressOf());

	// Render the triangle.
	deviceContext.DrawIndexed(indexCount, 0, 0);
}
