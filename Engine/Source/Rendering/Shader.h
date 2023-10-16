#pragma once

#include <string>
#include <wrl/client.h>

class GraphicsContext;
class GraphicsDevice;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

class Shader
{
public:
	Shader(const std::string& path, GraphicsDevice& graphicsDevice);
	void Render(GraphicsContext& graphicsContext) const;

private:
	GraphicsDevice& graphicsDevice;

	Handle vertexShader;
	Handle pixelShader;
	Handle layout;
};