#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <memory>
#include <wrl/client.h>

#include "Texture.h"

class Model
{
public:
	Model(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const char* textureFilename);

	void Render(ID3D11DeviceContext& deviceContext) const;
	int GetIndexCount() const;

	ID3D11ShaderResourceView& GetTexture() const;

private:
	void RenderBuffers(ID3D11DeviceContext& deviceContext) const;

	int vertexCount, indexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
	std::unique_ptr<Texture> texture;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
};
