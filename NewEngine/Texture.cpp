#include <comdef.h>
#include <stdexcept>

#include <DDSTextureLoader.h>

#include "Texture.h"

using namespace std;
using namespace DirectX;

Texture::Texture(ID3D11Device* device, WCHAR* filename)
{
	_com_util::CheckError(CreateDDSTextureFromFile(device, filename, NULL, &texture));
}

Texture::~Texture()
{
	texture->Release();
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture;
}
