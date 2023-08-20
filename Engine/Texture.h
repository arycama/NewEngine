#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

class Texture
{
public:
	Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const std::string& filename);

	ID3D11ShaderResourceView& GetTexture() const;

private:
	void LoadTarga(const std::string&, int&, int&);
	unsigned char* targaData;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;

	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
};
