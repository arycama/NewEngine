#include "Material.h"
#include "Shader.h"
#include "Texture.h"

#include <d3d11.h>
#include <minwindef.h>
#include <wrl/client.h>
#include <comdef.h>

using namespace _com_util;
using namespace std;

Material::Material(shared_ptr<const Texture> texture, shared_ptr<const Shader> shader, const string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : shader(shader), texture(texture), deviceContext(deviceContext), path(path)
{ 
	// Create a texture sampler state description.
	// Create the texture sampler state.
	FLOAT borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	CD3D11_SAMPLER_DESC samplerDesc(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, borderColor, 0, D3D11_FLOAT32_MAX);
	CheckError(device.CreateSamplerState(&samplerDesc, samplerState.GetAddressOf()));
}

const string& Material::GetPath() const
{
	return path;
}

void Material::Render() const
{
	shader->Render();

	auto shaderResourceViews = &texture->GetTexture();
	deviceContext.PSSetShaderResources(0, 1, &shaderResourceViews);
	deviceContext.PSSetSamplers(0, 1, samplerState.GetAddressOf());
}