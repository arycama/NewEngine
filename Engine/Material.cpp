#include "Material.h"
#include "Shader.h"
#include "Texture.h"

#include <comdef.h>
#include <d3d11.h>

using namespace _com_util;

Material::Material(const Shader& shader, const Texture& texture, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : shader(shader), texture(texture), deviceContext(deviceContext)
{ 
	// Create a texture sampler state description.
	// Create the texture sampler state.
	FLOAT borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	auto samplerDesc = CD3D11_SAMPLER_DESC(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, borderColor, 0, D3D11_FLOAT32_MAX);
	CheckError(device.CreateSamplerState(&samplerDesc, &samplerState));
}

void Material::Render() const
{
	shader.Render();

	auto shaderResourceViews = &texture.GetTexture();
	deviceContext.PSSetShaderResources(0, 1, &shaderResourceViews);
	deviceContext.PSSetSamplers(0, 1, samplerState.GetAddressOf());
}