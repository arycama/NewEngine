#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <memory>

#include "Texture.h"

class Model
{
public:
	Model(ID3D11Device*, WCHAR*);
	~Model();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	std::unique_ptr<Texture> texture;
};
