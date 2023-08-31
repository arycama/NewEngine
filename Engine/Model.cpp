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

	const auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	const auto vertexData = D3D11_SUBRESOURCE_DATA{ vertices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	auto indices = make_unique<int[]>(indexCount);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	const auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	const auto indexData = D3D11_SUBRESOURCE_DATA { indices.get(), 0, 0 };
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));
}

Model::Model(const string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext), vertexStride(sizeof(VertexType))
{
	// Open the model file.
	ifstream fin;
	fin.open(path);

	// If it could not open the file then exit.
	if (fin.fail())
		return;

	char input;
	fin.get(input);
#if 1

	auto positions = make_unique<vector<XMFLOAT3>>();
	auto uvs = make_unique<vector<XMFLOAT2>>();
	auto normals = make_unique<vector<XMFLOAT3>>();
	auto vertices = make_unique<vector<VertexType>>();
	auto indices = make_unique<vector<unsigned int>>();

	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			if (input == ' ')
			{
				XMFLOAT3 position;
				fin >> position.x >> position.y >> position.z;
				//position.z = -position.z;
				positions->push_back(position);
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				XMFLOAT2 uv;
				fin >> uv.x >> uv.y;
				//uv.y = 1.0f - uv.y;
				uvs->push_back(uv);
			}

			// Read in the normals.
			if (input == 'n')
			{
				XMFLOAT3 normal;
				fin >> normal.x >> normal.y >> normal.z;
				//normal.z = -normal.z;
				normals->push_back(normal);
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				unsigned int vIndex1, vIndex2, vIndex3, tIndex1, tIndex2, tIndex3, nIndex1, nIndex2, nIndex3;
				char input2;

				/*fin >> vIndex3 >> input2 >> tIndex3 >> input2 >> nIndex3
					>> vIndex2 >> input2 >> tIndex2 >> input2 >> nIndex2
					>> vIndex1 >> input2 >> tIndex1 >> input2 >> nIndex1;*/

				fin >> vIndex1 >> input2 >> tIndex1 >> input2 >> nIndex1
					>> vIndex2 >> input2 >> tIndex2 >> input2 >> nIndex2
					>> vIndex3 >> input2 >> tIndex3 >> input2 >> nIndex3;

				VertexType vertex0;
				vertex0.position = positions->at(vIndex1 - 1);
				vertex0.uv = uvs->at(tIndex1 - 1);
				vertex0.normal = normals->at(nIndex1 - 1);
				vertices->push_back(vertex0);
				indices->push_back(indices->size());

				VertexType vertex1;
				vertex1.position = positions->at(vIndex2 - 1);
				vertex1.uv = uvs->at(tIndex2 - 1);
				vertex1.normal = normals->at(nIndex2 - 1);
				vertices->push_back(vertex1);
				indices->push_back(indices->size());

				VertexType vertex2;
				vertex2.position = positions->at(vIndex3 - 1);
				vertex2.uv = uvs->at(tIndex3 - 1);
				vertex2.normal = normals->at(nIndex3 - 1);
				vertices->push_back(vertex2);
				indices->push_back(indices->size());
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
			fin.get(input);

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	fin.close();

	vertexCount = vertices->size();
	indexCount = indices->size();

	const auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);
	const auto vertexData = D3D11_SUBRESOURCE_DATA{ vertices->data(), 0, 0};
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	const auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	const auto indexData = D3D11_SUBRESOURCE_DATA{ indices->data(), 0, 0 };
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