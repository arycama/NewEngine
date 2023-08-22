#pragma once

#include "Component.h"

#include <directxmath.h>
#include <memory>
#include <string>
#include <wrl/client.h>

class Model : public Component
{
public:
	Model(struct ID3D11Device& device, struct ID3D11DeviceContext& deviceContext);

	void Render() const;

private:
	int vertexCount, indexCount;
	struct ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<struct ID3D11Buffer> vertexBuffer, indexBuffer;
	
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
};
