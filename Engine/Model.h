#pragma once

#include "Component.h"

#include <directxmath.h>
#include <memory>
#include <string>
#include <wrl/client.h>

class Model : public Component
{
public:
	Model(struct ID3D11Device& device, struct ID3D11DeviceContext& deviceContext, const std::string& textureFilename);

	void Render(struct ID3D11DeviceContext& deviceContext) const;
	int GetIndexCount() const;

	struct ID3D11ShaderResourceView& GetTexture() const;

private:
	void RenderBuffers(struct ID3D11DeviceContext& deviceContext) const;

	int vertexCount, indexCount;

	Microsoft::WRL::ComPtr<struct ID3D11Buffer> vertexBuffer, indexBuffer;
	std::unique_ptr<class Texture> texture;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
};
