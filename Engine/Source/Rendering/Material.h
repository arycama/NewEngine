#pragma once

#include <memory>
#include <string>
#include <wrl/client.h>

class GraphicsContext;
class GraphicsDevice;
class Shader;
class Texture;
struct ID3D11SamplerState;

class Material
{
public:
	Material(std::shared_ptr<const Texture> texture, std::shared_ptr<const Shader> shader, const std::string& path, GraphicsDevice& graphicsDevice);

	const std::string& GetPath() const;
	void Render(GraphicsContext& graphicsContext) const;

private:

	GraphicsDevice& graphicsDevice;

	const std::string path;
	const std::shared_ptr<const Shader> shader;
	const std::shared_ptr<const Texture> texture; // Todo this should probably be a vector

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};