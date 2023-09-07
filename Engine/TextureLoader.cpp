#include "TextureFormat.h"
#include "TextureLoader.h"
#include "TgaLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assert.h>

using namespace std;

TextureLoader::TextureLoader()
{
	stbi_set_flip_vertically_on_load(true);
}

unique_ptr<unsigned char[]> TextureLoader::LoadTexture(const string& path, int& width, int& height) const
{
	return unique_ptr<unsigned char[]>(stbi_load(path.c_str(), &width, &height, 0, STBI_rgb_alpha));
}
