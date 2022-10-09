#pragma once

#include <d3d11.h>

class Texture
{
public:
	Texture(ID3D11Device*, WCHAR*);
	~Texture();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* texture;

};
