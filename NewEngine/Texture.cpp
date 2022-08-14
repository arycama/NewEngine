#include "Texture.h"

Texture::Texture(ID3D11Device* device, WCHAR* filename)
{
	auto result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture, NULL);

	if (FAILED(result))
		throw "SRV Creation Failed";
}

Texture::~Texture()
{
	texture->Release();
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture;
}
