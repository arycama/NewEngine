#include "Model.h"

#include <d3d11.h>
#include <memory>
#include <comdef.h>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace _com_util;

Model::Model(ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext)
{
	// Initialize the vertex and index buffers.
	// Set the number of vertices in the vertex array.
	vertexCount = 3;

	// Create the vertex array.
	auto vertices = make_unique<VertexType[]>(vertexCount);

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	// Set the number of indices in the index array.
	indexCount = 3;

	// Create the index array.
	auto indices = make_unique<unsigned long[]>(indexCount);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	const auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(VertexType) * vertexCount, D3D11_BIND_VERTEX_BUFFER);

	// Give the subresource structure a pointer to the vertex data.
	const auto vertexData = D3D11_SUBRESOURCE_DATA{ vertices.get(), 0, 0 };

	// Now create the vertex buffer.
	CheckError(device.CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer));

	// Set up the description of the static index buffer.
	const auto indexBufferDesc = CD3D11_BUFFER_DESC(sizeof(unsigned long) * indexCount, D3D11_BIND_INDEX_BUFFER);;

	// Give the subresource structure a pointer to the index data.
	const auto indexData = D3D11_SUBRESOURCE_DATA { indices.get(), 0, 0 };

	// Create the index buffer.
	CheckError(device.CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));
}

void Model::Render()
{
	// Set vertex buffer stride and offset.
	constexpr auto stride = static_cast<UINT>(sizeof(VertexType));
	constexpr auto offset = 0u;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext.IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext.IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Model::GetIndexCount() const
{
	return indexCount;
}
