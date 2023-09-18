#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Graphics;
class Material;
class Model;
class Shader;
class Texture;
class TextureLoader;

class ResourceManager
{
public:
	ResourceManager(const TextureLoader& texureLoader, Graphics& graphics);

	std::shared_ptr<const Material> LoadMaterial(const std::string& path);
	std::shared_ptr<const Model> LoadModel(const std::string& path);
	std::shared_ptr<const Shader> LoadShader(const std::string& path);
	std::shared_ptr<const Texture> LoadTexture(const std::string& path);

private:
	const TextureLoader& textureLoader;
	Graphics& graphics;

	std::unordered_map<std::string, std::weak_ptr<Material>> materials;
	std::unordered_map<std::string, std::weak_ptr<Model>> models;
	std::unordered_map<std::string, std::weak_ptr<Shader>> shaders;
	std::unordered_map<std::string, std::weak_ptr<Texture>> textures;
};