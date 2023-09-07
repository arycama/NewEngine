#pragma once

#include <wrl/client.h>

class Shader;
class Texture;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;

class Material
{
public:
	Material(const Texture& texture, const Shader& shader, ID3D11Device& device, ID3D11DeviceContext& deviceContext);

	void Render() const;

private:
	const Shader& shader;
	const Texture& texture; // Todo this should probably be a vector
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};