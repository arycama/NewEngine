#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Material;
class Shader;
class Texture;
class TextureLoader;

struct ID3D11Device;
struct ID3D11DeviceContext;

class ResourceManager
{
public:
	ResourceManager(const TextureLoader& texureLoader, ID3D11Device& device, ID3D11DeviceContext& context);

	std::shared_ptr<Material> LoadMaterial(const std::string& path);
	std::shared_ptr<Shader> LoadShader(const std::string& path);
	std::shared_ptr<Texture> LoadTexture(const std::string& path);

private:
	const TextureLoader& textureLoader;
	ID3D11Device& device;
	ID3D11DeviceContext& context;

	std::unordered_map<std::string, std::shared_ptr<Material>> materials;
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};