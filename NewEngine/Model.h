#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

class Model
{
public:
	Model(ID3D11Device*);
	~Model();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
};
