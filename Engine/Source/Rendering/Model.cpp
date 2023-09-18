#include "GraphicsDevice.h"
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

Model::Model(const string& path, GraphicsDevice& graphicsDevice) : graphicsDevice(graphicsDevice), vertexStride(sizeof(VertexType)), path(path)
{
	ifstream file(path);
	assert(file.is_open());

	char input;
	file.get(input);

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

						VertexType vertex{ positions->at(vIndex - 1), normals->at(nIndex - 1), uvs->at(tIndex - 1) };
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
	CheckError(graphicsDevice.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * indexCount, D3D11_BIND_INDEX_BUFFER);;
	D3D11_SUBRESOURCE_DATA indexData{ indices->data(), 0, 0 };
	CheckError(graphicsDevice.GetDevice().CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));
}

const std::string& Model::GetPath() const
{
	return path;
}

void Model::Render() const
{
	// Set vertex buffer stride and offset.
	auto stride = static_cast<UINT>(vertexStride);
	auto offset = 0u;

	auto& context = graphicsDevice.GetDeviceContext();
	context.IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context.IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context.DrawIndexed(indexCount, 0, 0);
}