#pragma once

#include <memory>
#include <string>
#include <wrl/client.h>

class Shader;
class Texture;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;

class Material
{
public:
	Material(std::shared_ptr<const Texture> texture, std::shared_ptr<const Shader> shader, const std::string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext);

	const std::string& GetPath() const;
	void Render() const;

private:
	const std::string& path;

	const std::shared_ptr<const Shader> shader;
	const std::shared_ptr<const Texture> texture; // Todo this should probably be a vector
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};