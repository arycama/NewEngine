#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>

class Texture
{
public:
	Texture(ID3D11Device*, WCHAR*);
	~Texture();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* texture;

};
