#include "GraphicsContext.h"
#include "GraphicsDevice.h"
#include "Texture.h"
#include "TextureFormat.h"

Texture::Texture(unsigned char* data, int width, int height, GraphicsDevice& graphicsDevice, GraphicsContext& graphicsContext)
{
	graphicsDevice.CreateTexture2D(width, height, texture.GetAddressOf());
	graphicsDevice.CreateShaderResourceView(*texture.Get(), TextureFormat::R8G8B8A8Unorm, textureView.GetAddressOf());

	graphicsContext.UpdateSubresource(*texture.Get(), 0, nullptr, data, width * 4, 0);
	graphicsContext.GenerateMips(*textureView.Get());
}

ID3D11ShaderResourceView& Texture::GetTexture() const
{
	return *textureView.Get();
}
