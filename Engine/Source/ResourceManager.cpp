#include "Material.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureLoader.h"

#include <d3d11.h>
#include <assert.h>
#include <fstream>

using namespace std;

ResourceManager::ResourceManager(const TextureLoader& texureLoader, ID3D11Device& device, ID3D11DeviceContext& context) :textureLoader(textureLoader), device(device), context(context) { }

shared_ptr<Material> ResourceManager::LoadMaterial(const string& path)
{
	const auto result = materials.find(path);
	if (result != materials.end())
		return result->second;

	ifstream file(path);
	assert(file.is_open());

	shared_ptr<Shader> shader;
	shared_ptr<Texture> texture;

	string line;
	while (getline(file, line))
	{
		auto pos = line.find_first_of(':');
		assert(pos != string::npos);
		auto variable = line.substr(0, pos);

		if (variable == "texture")
		{
			auto start = line.find_first_of('"');
			assert(start != string::npos);

			auto end = line.find_last_of('"');
			assert(end != string::npos);

			auto path = line.substr(start, end - start);
			texture = LoadTexture(path);
		}

		if (variable == "shader")
		{
			auto start = line.find_first_of('"');
			assert(start != string::npos);

			auto end = line.find_last_of('"');
			assert(end != string::npos);

			auto path = line.substr(start, end - start);
			shader = LoadShader(path);
		}
	}

	auto material = make_shared<Material>(texture, shader, device, context);
	materials.insert_or_assign(path, material);
	return material;
}

shared_ptr<Shader> ResourceManager::LoadShader(const string& path)
{
	const auto result = shaders.find(path);
	if (result != shaders.end())
		return result->second;

	auto shader = make_shared<Shader>("Shaders/Surface.hlsl", device, context);
	shaders.insert_or_assign(path, shader);
	return shader;
}

shared_ptr<Texture> ResourceManager::LoadTexture(const string& path)
{
	const auto result = textures.find(path);
	if (result != textures.end())
		return result->second;

	int width = 0, height = 0;
	const auto textureData = textureLoader.LoadTexture("Assets/Stones/STONE#1/STONE#1_Textures/STONE#1_color.png", width, height);

	auto texture = make_shared<Texture>(textureData.get(), width, height, device, context);
	textures.insert_or_assign(path, texture);
	return texture;
}
