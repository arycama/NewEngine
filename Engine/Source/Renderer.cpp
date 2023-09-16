#include "Engine.h"
#include "Entity.h"
#include "Material.h"
#include "Model.h"
#include "Renderer.h"
#include "ResourceManager.h"
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

Renderer::Renderer(shared_ptr<const Model> model, std::shared_ptr<const Material> material, const Transform& transform, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext, const Entity& entity) : model(model), material(material), transform(&transform), engine(engine), deviceContext(deviceContext), entity(entity)
{
	engine.AddRenderer(*this);

	CD3D11_BUFFER_DESC drawDataDesc(sizeof(PerDrawData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&drawDataDesc, nullptr, &drawData));
	SetDebugObjectName(drawData.Get(), "Draw Data");
}

Renderer::Renderer(istream& stream, ResourceManager& resourceManager, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext, const Entity& entity) : engine(engine), deviceContext(deviceContext), entity(entity)
{
	int transformIndex;
	stream >> transformIndex;
	transform = &dynamic_cast<Transform&>(entity.GetComponentAt(transformIndex));

	string modelPath;
	stream >> modelPath;
	model = resourceManager.LoadModel(modelPath);

	string materialPath;
	stream >> materialPath;
	material = resourceManager.LoadMaterial(materialPath);

	engine.AddRenderer(*this);

	CD3D11_BUFFER_DESC drawDataDesc(sizeof(PerDrawData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&drawDataDesc, nullptr, &drawData));
	SetDebugObjectName(drawData.Get(), "Draw Data");
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Serialize(ostream& stream) const
{
	stream << "renderer" << ' ';
	stream << entity.GetComponentIndex(*transform) << ' ';
	stream << model->GetPath() << ' ';
	stream << material->GetPath();
}

void Renderer::Render() const
{
	D3D11_MAPPED_SUBRESOURCE perDrawDataMappedResource;
	CheckError(deviceContext.Map(drawData.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &perDrawDataMappedResource));

	// Get a pointer to the data in the constant buffer.
	auto drawDataPtr = static_cast<PerDrawData*>(perDrawDataMappedResource.pData);

	// Copy the matrices into the constant buffer.
	drawDataPtr->model = transform->GetWorldMatrix();

	// Unlock the constant buffer.
	deviceContext.Unmap(drawData.Get(), 0);

	material->Render();
	deviceContext.VSSetConstantBuffers(1, 1, drawData.GetAddressOf());
	model->Render();
}
