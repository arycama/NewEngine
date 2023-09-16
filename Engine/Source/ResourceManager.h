#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Material;
class Model;
class Shader;
class Texture;
class TextureLoader;

struct ID3D11Device;
struct ID3D11DeviceContext;

class ResourceManager
{
public:
	ResourceManager(const TextureLoader& texureLoader, ID3D11Device& device, ID3D11DeviceContext& context);

	std::shared_ptr<const Material> LoadMaterial(const std::string& path);
	std::shared_ptr<const Model> LoadModel(const std::string& path);
	std::shared_ptr<const Shader> LoadShader(const std::string& path);
	std::shared_ptr<const Texture> LoadTexture(const std::string& path);

private:
	const TextureLoader& textureLoader;
	ID3D11Device& device;
	ID3D11DeviceContext& context;

	std::unordered_map<std::string, std::weak_ptr<Material>> materials;
	std::unordered_map<std::string, std::weak_ptr<Model>> models;
	std::unordered_map<std::string, std::weak_ptr<Shader>> shaders;
	std::unordered_map<std::string, std::weak_ptr<Texture>> textures;
};