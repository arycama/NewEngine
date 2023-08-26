#pragma once

#include "Component.h"
#include <wrl/client.h>

class Transform;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Model : public Component
{
public:
	Model(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const Transform& transform);

	void Render() const;

private:
	const int vertexCount, indexCount;
	const Transform& transform;
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
};
