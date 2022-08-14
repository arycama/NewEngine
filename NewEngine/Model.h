#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

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
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
	};

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	Texture* texture;
};
