#include "Texture.h"

#include <comdef.h>
#include <cstdio>
#include <memory>

using namespace _com_util;
using namespace Microsoft::WRL;
using namespace std;

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

Texture::Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const string& filename)
{
	FILE* filePtr;
	auto error = fopen_s(&filePtr, filename.c_str(), "rb");
	assert(!error);

	TargaHeader tgaHeader;
	auto headerSize = static_cast<unsigned int>(fread(&tgaHeader, sizeof(TargaHeader), 1, filePtr));
	assert(headerSize == 1);
	assert(tgaHeader.bpp == 32);

	auto imageSize = tgaHeader.width * tgaHeader.height * 4;
	auto targaImage = make_unique<unsigned char[]>(imageSize);

	auto pixelCount = fread(targaImage.get(), 1, imageSize, filePtr);
	assert(pixelCount == imageSize);

	auto closeError = fclose(filePtr);
	assert(!closeError);

	auto textureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, tgaHeader.width, tgaHeader.height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS);
	CheckError(device.CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()));

	auto rowPitch = (tgaHeader.width * 4) * sizeof(unsigned char);
	deviceContext.UpdateSubresource(texture.Get(), 0, nullptr, targaImage.get(), rowPitch, 0);

	auto srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format, 0, -1);
	CheckError(device.CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf()));

	deviceContext.GenerateMips(textureView.Get());
}

ID3D11ShaderResourceView& Texture::GetTexture() const
{
	return *textureView.Get();
}
