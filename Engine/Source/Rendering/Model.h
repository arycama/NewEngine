#pragma once

#include <string>
#include <wrl/client.h>

class GraphicsContext;
class GraphicsDevice;
struct ID3D11Buffer;

class Model
{
public:
	Model(const std::string& path, GraphicsDevice& graphicsDevice);
	void Render(GraphicsContext& graphicsContext) const;

	const std::string& GetPath() const;

private:
	const std::string path;
	int vertexCount, vertexStride, indexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
};