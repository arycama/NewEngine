#include "Model.h"
#include "Transform.h"

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <memory>
#include <dxgiformat.h>
#include <minwindef.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <comdef.h>
#include <string>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace _com_util;

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct ModelType
{
	XMFLOAT3 position, normal;
	XMFLOAT2 uv;
};

Model::Model(ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext), vertexCount(3), indexCount(3), vertexStride(sizeof(VertexType))
{
	auto vertices = make_unique<VertexType[]>(vertexCount);
	vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	vertices[1] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) };
	vertices[2] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

	const auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	const auto vertexData = D3D11_SUBRESOURCE_DATA{ vertices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer));

	auto indices = make_unique<int[]>(indexCount);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	const auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	const auto indexData = D3D11_SUBRESOURCE_DATA { indices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));
}

Model::Model(const string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext), vertexStride(sizeof(ModelType))
{
	// Open the model file.
	ifstream fin;
	fin.open(path);

	// If it could not open the file then exit.
	if (fin.fail())
		return;

	// Read up to the value of vertex count.
	char input;
	fin.get(input);
	while (input != ':')
		fin.get(input);

	// Read in the vertex count.
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	auto vertices = make_unique<ModelType[]>(vertexCount);
	auto indices = make_unique<int[]>(indexCount);
	for (auto i = 0; i < vertexCount; i++)
	{
		fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
		fin >> vertices[i].uv.x >> vertices[i].uv.y;
		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;

		indices[i] = i;
	}

	fin.close();

	const auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(ModelType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	const auto vertexData = D3D11_SUBRESOURCE_DATA{ vertices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer));

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	const auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	const auto indexData = D3D11_SUBRESOURCE_DATA{ indices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));
}

void Model::Render() const
{
	// Set vertex buffer stride and offset.
	auto stride = static_cast<UINT>(vertexStride);
	auto offset = 0u;

	deviceContext.IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext.IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext.DrawIndexed(indexCount, 0, 0);
}