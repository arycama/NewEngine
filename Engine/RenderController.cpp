#include "Camera.h"
#include "RenderController.h"

#include <comdef.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include <memory>

using namespace _com_util;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

struct PerCameraData
{
	XMMATRIX view;
	XMMATRIX projection;
};

struct PerDrawData
{
	XMMATRIX model;
};

RenderController::RenderController(ID3D11Device& device, ID3D11DeviceContext& deviceContext) : deviceContext(deviceContext)
{
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	auto cameraDataDesc = CD3D11_BUFFER_DESC(sizeof(PerCameraData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&cameraDataDesc, nullptr, &cameraData));

	auto drawDataDesc = CD3D11_BUFFER_DESC(sizeof(PerDrawData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&drawDataDesc, nullptr, &drawData));
}

void RenderController::UpdateCameraData(const Camera& camera)
{
	// Set the shader parameters that it will use for rendering.
	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE perCameraDataMappedResource;
	CheckError(deviceContext.Map(cameraData.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &perCameraDataMappedResource));

	// Get a pointer to the data in the constant buffer.
	auto perCameraDataPtr = static_cast<PerCameraData*>(perCameraDataMappedResource.pData);

	// Copy the matrices into the constant buffer.
	perCameraDataPtr->view = camera.GetViewMatrix();
	perCameraDataPtr->projection = camera.GetProjectionMatrix();

	// Unlock the constant buffer.
	deviceContext.Unmap(cameraData.Get(), 0);
}

void RenderController::UpdateDrawData(XMMATRIX model)
{
	D3D11_MAPPED_SUBRESOURCE perDrawDataMappedResource;
	CheckError(deviceContext.Map(drawData.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &perDrawDataMappedResource));

	// Get a pointer to the data in the constant buffer.
	auto drawDataPtr = static_cast<PerDrawData*>(perDrawDataMappedResource.pData);

	// Copy the matrices into the constant buffer.
	drawDataPtr->model = model;

	// Unlock the constant buffer.
	deviceContext.Unmap(drawData.Get(), 0);
}

ID3D11Buffer& RenderController::GetPerCameraData() const
{
	return *cameraData.Get();
}

ID3D11Buffer& RenderController::GetPerDrawData() const
{
	return *drawData.Get();
}
