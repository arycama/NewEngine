#include "GraphicsDevice.h"
#include "Material.h"
#include "Rendering/Model.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureLoader.h"

#include <d3d11.h>
#include <assert.h>
#include <fstream>

using namespace std;

ResourceManager::ResourceManager(const TextureLoader& texureLoader, GraphicsDevice& graphicsDevice) : textureLoader(textureLoader), graphicsDevice(graphicsDevice) { }

shared_ptr<const Material> ResourceManager::LoadMaterial(const string& path)
{
	const auto result = materials.find(path);
	if (result != materials.end() && !result->second.expired())
		return result->second.lock();

	ifstream file(path);
	assert(file.is_open());

	string texturePath, shaderPath;
	file >> texturePath >> shaderPath;

	auto texture = LoadTexture(texturePath);
	auto shader = LoadShader(shaderPath);

	auto material = make_shared<Material>(texture, shader, path, graphicsDevice);
	materials.insert_or_assign(path, material);
	return material;
}

shared_ptr<const Model> ResourceManager::LoadModel(const string& path)
{
	const auto result = models.find(path);
	if (result != models.end() && !result->second.expired())
		return result->second.lock();

	auto model = make_shared<Model>(path, graphicsDevice);
	models.insert_or_assign(path, model);
	return model;
}

shared_ptr<const Shader> ResourceManager::LoadShader(const string& path)
{
	const auto result = shaders.find(path);
	if (result != shaders.end() && !result->second.expired())
		return result->second.lock();

	auto shader = make_shared<Shader>(path, graphicsDevice);
	shaders.insert_or_assign(path, shader);
	return shader;
}

shared_ptr<const Texture> ResourceManager::LoadTexture(const string& path)
{
	const auto result = textures.find(path);
	if (result != textures.end() && !result->second.expired())
		return result->second.lock();

	int width = 0, height = 0;
	const auto textureData = textureLoader.LoadTexture(path, width, height);

	auto texture = make_shared<Texture>(textureData.get(), width, height, graphicsDevice, graphicsDevice.GetGraphicsContext());
	textures.insert_or_assign(path, texture);
	return texture;
}
