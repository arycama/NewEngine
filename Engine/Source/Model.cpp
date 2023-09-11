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
#include <vector>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace _com_util;

struct VertexType
{
	XMFLOAT3 position, normal;
	XMFLOAT2 uv;
};

Model::Model(ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext), vertexCount(3), indexCount(3), vertexStride(sizeof(VertexType))
{
	auto vertices = make_unique<VertexType[]>(vertexCount);
	vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) };
	vertices[1] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2(0.5f, 0.0f) };
	vertices[2] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2(1.0f, 1.0f) };

	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vertexData{ vertices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	auto indices = make_unique<int[]>(indexCount);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	D3D11_SUBRESOURCE_DATA indexData{ indices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));
}

Model::Model(const string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext), vertexStride(sizeof(VertexType))
{
	ifstream file(path);
	assert(file.is_open());

	char input;
	file.get(input);
#if 1

	auto positions = make_unique<vector<XMFLOAT3>>();
	auto uvs = make_unique<vector<XMFLOAT2>>();
	auto normals = make_unique<vector<XMFLOAT3>>();
	auto vertices = make_unique<vector<VertexType>>();
	auto indices = make_unique<vector<unsigned int>>();

	while (!file.eof())
	{
		switch (input)
		{
			case 'v':
			{
				file.get(input);

				switch (input)
				{
					case ' ':
					{
						// Normal
						float x, y, z;
						file >> x >> y >> z;
						positions->push_back(XMFLOAT3(x, y, z));
						break;
					}
					case 't':
					{
						// Uv
						float x, y;
						file >> x >> y;
						uvs->push_back(XMFLOAT2(x, y));
						break;
					}
					case 'n':
					{
						// Normal
						float x, y, z;
						file >> x >> y >> z;
						normals->push_back(XMFLOAT3(x, y, z));
						break;
					}
				}

				break;
			}

			case 'f':
			{
				// Face
				file.get(input);
				if (input == ' ')
				{
					for (auto i = 0; i < 3; i++)
					{
						char input2;
						unsigned int vIndex, tIndex, nIndex;
						file >> vIndex >> input2 >> tIndex >> input2 >> nIndex;

						VertexType vertex;
						vertex.position = positions->at(vIndex - 1);
						vertex.uv = uvs->at(tIndex - 1);
						vertex.normal = normals->at(nIndex - 1);
						vertices->push_back(vertex);
						indices->push_back(indices->size());
					}
				}

				break;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
			file.get(input);

		// Start reading the beginning of the next line.
		file.get(input);
	}

	vertexCount = vertices->size();
	indexCount = indices->size();

	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vertexData{ vertices->data(), 0, 0 };
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	D3D11_SUBRESOURCE_DATA indexData{ indices->data(), 0, 0 };
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));

#else
	// Read up to the value of vertex count.
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
	auto vertices = make_unique<VertexType[]>(vertexCount);
	auto indices = make_unique<int[]>(indexCount);
	for (auto i = 0; i < vertexCount; i++)
	{
		fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
		fin >> vertices[i].uv.x >> vertices[i].uv.y;
		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;

		indices[i] = i;
	}

	fin.close();

	const auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	const auto vertexData = D3D11_SUBRESOURCE_DATA{ vertices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	const auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	const auto indexData = D3D11_SUBRESOURCE_DATA{ indices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));
#endif
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