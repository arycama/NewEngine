#pragma once

#include "Component.h"
#include <string>
#include <wrl/client.h>

class Transform;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Model : public Component
{
public:
	Model(ID3D11Device& device, ID3D11DeviceContext& deviceContext);
	Model(const std::string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext);

	void Render() const;

private:
	int vertexCount, vertexStride, indexCount;
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
};