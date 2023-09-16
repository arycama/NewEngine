#pragma once

#include "Component.h"
#include <string>
#include <wrl/client.h>

class Graphics;
class Transform;
struct ID3D11Buffer;

class Model
{
public:
	Model(const std::string& path, Graphics& graphics);
	void Render() const;

	const std::string& GetPath() const;

private:
	Graphics& graphics;
	const std::string path;
	int vertexCount, vertexStride, indexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
};