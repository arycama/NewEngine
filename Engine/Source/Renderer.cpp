#include "Engine.h"
#include "Material.h"
#include "Model.h"
#include "Renderer.h"
#include "Transform.h"
#include "DirectXHelpers.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <comdef.h>

using namespace _com_util;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

struct PerDrawData
{
	XMMATRIX model;
};

Renderer::Renderer(const Model& model, const Material& material, const Transform& transform, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : model(model), material(material), transform(transform), engine(engine), deviceContext(deviceContext)
{
	engine.AddRenderer(*this);

	auto drawDataDesc = CD3D11_BUFFER_DESC(sizeof(PerDrawData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&drawDataDesc, nullptr, &drawData));
	SetDebugObjectName(drawData.Get(), "Draw Data");
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Render() const
{
	D3D11_MAPPED_SUBRESOURCE perDrawDataMappedResource;
	CheckError(deviceContext.Map(drawData.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &perDrawDataMappedResource));

	// Get a pointer to the data in the constant buffer.
	auto drawDataPtr = static_cast<PerDrawData*>(perDrawDataMappedResource.pData);

	// Copy the matrices into the constant buffer.
	drawDataPtr->model = transform.GetWorldMatrix();

	// Unlock the constant buffer.
	deviceContext.Unmap(drawData.Get(), 0);

	material.Render();
	deviceContext.VSSetConstantBuffers(1, 1, drawData.GetAddressOf());
	model.Render();
}
