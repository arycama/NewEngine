#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

class GraphicsContext;
class GraphicsDevice;

class Texture
{
public:
	Texture(unsigned char* data, int width, int height, GraphicsDevice& graphicsDevice, GraphicsContext& graphicsContext);

	ID3D11ShaderResourceView& GetTexture() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
};
