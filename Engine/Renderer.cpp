#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Model.h"
#include "RenderController.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include <comdef.h>
#include <DirectXMath.h>

using namespace _com_util;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

Renderer::Renderer(Model& model, const Shader& shader, const Graphics& graphics, Engine& engine, const Texture& texture, Transform& transform, RenderController& renderController) : model(model), shader(shader), graphics(graphics), engine(engine), texture(texture), transform(transform), renderController(renderController)
{
	engine.AddRenderer(*this);
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Render() const
{
	// Set the shader parameters that it will use for rendering.
	// Lock the constant buffer so it can be written to.
	//D3D11_MAPPED_SUBRESOURCE perCameraDataMappedResource;
	//CheckError(deviceContext.Map(perCameraData.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &perCameraDataMappedResource));

	//// Get a pointer to the data in the constant buffer.
	//auto perCameraDataPtr = static_cast<PerCameraData*>(perCameraDataMappedResource.pData);

	//// Copy the matrices into the constant buffer.
	//perCameraDataPtr->view = viewMatrix;
	//perCameraDataPtr->projection = projectionMatrix;

	//// Unlock the constant buffer.
	//deviceContext.Unmap(perCameraData.Get(), 0);

	//deviceContext.VSSetConstantBuffers(0, 1, perCameraData.GetAddressOf());

	//// Set the vertex input layout.
	//deviceContext.IASetInputLayout(layout.Get());
	//deviceContext.VSSetShader(vertexShader.Get(), nullptr, 0);

	//// Set shader texture resource in the pixel shader.
	//deviceContext.PSSetShader(pixelShader.Get(), nullptr, 0);

	//auto shaderResourceViews = &texture;
	//deviceContext.PSSetShaderResources(0, 1, &shaderResourceViews);
	//deviceContext.PSSetSamplers(0, 1, samplerState.GetAddressOf());

	renderController.UpdateDrawData(transform.GetWorldMatrix());
	shader.Render(texture.GetTexture());
	model.Render();
}
