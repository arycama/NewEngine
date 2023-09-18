#pragma once

#include "Component.h"
#include <string>
#include <wrl/client.h>

class GraphicsDevice;
class Transform;
struct ID3D11Buffer;

class Model
{
public:
	Model(const std::string& path, GraphicsDevice& graphicsDevice);
	void Render() const;

	const std::string& GetPath() const;

private:
	GraphicsDevice& graphicsDevice;
	const std::string path;
	int vertexCount, vertexStride, indexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
};