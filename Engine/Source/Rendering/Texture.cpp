#include "GraphicsContext.h"
#include "GraphicsDevice.h"
#include "Texture.h"
#include "TextureFormat.h"

Texture::Texture(unsigned char* data, int width, int height, GraphicsDevice& graphicsDevice, GraphicsContext& graphicsContext)
{
	texture = graphicsDevice.CreateTexture2D(width, height);
	textureView = graphicsDevice.CreateShaderResourceView(texture, TextureFormat::R8G8B8A8Unorm);

	graphicsContext.UpdateTextureSubresource(texture, 0, nullptr, data, width * 4, 0);
	graphicsContext.GenerateMips(textureView);
}

const Handle Texture::GetShaderResourceView() const
{
	return textureView;
}
