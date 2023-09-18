#include "Engine.h"
#include "Entity.h"
#include "GraphicsContext.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "Rendering/Model.h"
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

Renderer::Renderer(shared_ptr<const Model> model, std::shared_ptr<const Material> material, const Transform& transform, Engine& engine, GraphicsDevice& graphicsDevice, const Entity& entity) : model(model), material(material), transform(&transform), engine(engine), graphicsDevice(graphicsDevice), entity(entity)
{
	engine.AddRenderer(*this);

	CD3D11_BUFFER_DESC drawDataDesc(sizeof(PerDrawData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(graphicsDevice.GetDevice().CreateBuffer(&drawDataDesc, nullptr, &drawData));
	SetDebugObjectName(drawData.Get(), "Draw Data");
}

Renderer::Renderer(istream& stream, ResourceManager& resourceManager, Engine& engine, GraphicsDevice& graphicsDevice, const Entity& entity) : engine(engine), graphicsDevice(graphicsDevice), entity(entity)
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
	CheckError(graphicsDevice.GetDevice().CreateBuffer(&drawDataDesc, nullptr, &drawData));
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

void Renderer::Render(GraphicsContext& graphicsContext) const
{
	D3D11_MAPPED_SUBRESOURCE perDrawDataMappedResource;
	graphicsContext.BeginWrite(drawData.Get(), &perDrawDataMappedResource);

	auto drawDataPtr = static_cast<PerDrawData*>(perDrawDataMappedResource.pData);
	drawDataPtr->model = transform->GetWorldMatrix();

	graphicsContext.EndWrite(drawData.Get());

	material->Render(graphicsContext);
	graphicsContext.VSSetConstantBuffers(1, 1, drawData.GetAddressOf());
	model->Render(graphicsContext);
}