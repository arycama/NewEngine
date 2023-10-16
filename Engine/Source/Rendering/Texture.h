#pragma once

#include "Handle.h"

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

class GraphicsContext;
class GraphicsDevice;

class Texture
{
public:
	Texture(unsigned char* data, int width, int height, GraphicsDevice& graphicsDevice, GraphicsContext& graphicsContext);

	const Handle GetShaderResourceView() const;

private:
	Handle texture;
	Handle textureView;
};
