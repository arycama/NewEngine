#pragma once

#include <memory>
#include <string>
#include <wrl/client.h>

class Graphics;
class Shader;
class Texture;
struct ID3D11SamplerState;

class Material
{
public:
	Material(std::shared_ptr<const Texture> texture, std::shared_ptr<const Shader> shader, const std::string& path, Graphics& graphics);

	const std::string& GetPath() const;
	void Render() const;

private:

	Graphics& graphics;

	const std::string path;
	const std::shared_ptr<const Shader> shader;
	const std::shared_ptr<const Texture> texture; // Todo this should probably be a vector

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};