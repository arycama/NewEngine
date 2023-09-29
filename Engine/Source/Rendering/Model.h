#pragma once

#include "Handle.h"
#include <string>
#include <wrl/client.h>

class GraphicsContext;
class GraphicsDevice;

class Model
{
public:
	Model(const std::string& path, GraphicsDevice& graphicsDevice);
	void Render(GraphicsContext& graphicsContext) const;

	const std::string& GetPath() const;

private:
	const std::string path;
	int vertexCount, vertexStride, indexCount;

	Handle vertexBuffer, indexBuffer;
};