#include "Graphics.h"
#include "Shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <dxgiformat.h>
#include <d3d10shader.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <comdef.h>
#include <string>

using namespace _com_util;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

struct PerCameraData
{
	XMMATRIX view;
	XMMATRIX projection;
};

Shader::Shader(const string& path, Graphics& graphics) : graphics(graphics)
{
	auto fileName = wstring(path.begin(), path.end());

	// Create the vertex shader
	ComPtr<ID3D10Blob> vertexShaderBuffer;
	CheckError(D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "Vertex", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), nullptr));

	auto& device = graphics.GetDevice();
	CheckError(device.CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, vertexShader.GetAddressOf()));

	// TODO: Move to model?
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the Model and in the shader.
	constexpr int layoutSize = 3;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[layoutSize];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Create the vertex input layout.
	CheckError(device.CreateInputLayout(polygonLayout, layoutSize, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), layout.GetAddressOf()));

	// Create the pixel shader
	ComPtr<ID3D10Blob> pixelShaderBuffer;
	CheckError(D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "Pixel", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), nullptr));
	CheckError(device.CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, pixelShader.GetAddressOf()));
}

void Shader::Render() const
{
	auto& context = graphics.GetDeviceContext();
	context.IASetInputLayout(layout.Get());
	context.VSSetShader(vertexShader.Get(), nullptr, 0);
	context.PSSetShader(pixelShader.Get(), nullptr, 0);
}
