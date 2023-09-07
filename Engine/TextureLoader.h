#pragma once

#include <memory>
#include <string>

enum class TextureFormat;
class Texture;

class TextureLoader
{
public:
	TextureLoader();

	std::unique_ptr<unsigned char[]> LoadTexture(const std::string& path, int& width, int& height) const;
};