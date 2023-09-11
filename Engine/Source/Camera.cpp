#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Transform.h"

#include <d3d11.h>
#include "DirectXHelpers.h"
#include <DirectXMath.h>
#include <DirectXMathConvert.inl>
#include <DirectXMathMatrix.inl>
#include <DirectXMathMisc.inl>
#include <DirectXMathVector.inl>
#include <wrl/client.h>
#include <comdef.h>

using namespace _com_util;
using namespace DirectX;

struct PerCameraData
{
	XMMATRIX view;
	XMMATRIX projection;
};

Camera::Camera(float nearClipPlane, float farClipPlane, float fieldOfView, const Transform& transform, const Graphics& graphics, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext) : transform(transform), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane), fieldOfView(fieldOfView), graphics(graphics), engine(engine), deviceContext(deviceContext)
{ 
	engine.AddCamera(*this);
	CD3D11_BUFFER_DESC cameraDataDesc(sizeof(PerCameraData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CheckError(device.CreateBuffer(&cameraDataDesc, nullptr, &cameraData));
	SetDebugObjectName(cameraData.Get(), "Camera Data");
}

Camera::~Camera()
{
	engine.RemoveCamera(*this);
}

XMMATRIX Camera::GetViewMatrix() const
{
	// Setup the position of the camera in the world and load it into a XMVECTOR structure.
	auto position = transform.GetPosition();
	auto positionVector = XMLoadFloat3(&position);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	auto rotation = transform.GetRotation();
	auto rotationVector = XMLoadFloat3(&rotation);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	auto quaternionVector = XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	XMFLOAT3 forward(0.0f, 0.0f, 1.0f);
	auto lookAtVector = XMLoadFloat3(&forward);
	lookAtVector = XMVector3Rotate(lookAtVector, quaternionVector);

	XMFLOAT3 up(0.0f, 1.0f, 0.0f);
	auto upVector = XMLoadFloat3(&up);
	upVector = XMVector3Rotate(upVector, quaternionVector);

	return XMMatrixLookToLH(positionVector, lookAtVector, upVector);
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	const auto aspect = graphics.GetAspectRatio();
	const auto fovRadians = XMConvertToRadians(fieldOfView);
	return XMMatrixPerspectiveFovLH(fovRadians, aspect, nearClipPlane, farClipPlane);
}

void Camera::Render() const
{
	// Set the shader parameters that it will use for rendering.
	D3D11_MAPPED_SUBRESOURCE perCameraDataMappedResource;
	CheckError(deviceContext.Map(cameraData.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &perCameraDataMappedResource));

	// Get a pointer to the data in the constant buffer.
	auto perCameraDataPtr = static_cast<PerCameraData*>(perCameraDataMappedResource.pData);

	// Copy the matrices into the constant buffer.
	perCameraDataPtr->view = GetViewMatrix();
	perCameraDataPtr->projection = GetProjectionMatrix();

	// Unlock the constant buffer.
	deviceContext.Unmap(cameraData.Get(), 0);

	deviceContext.VSSetConstantBuffers(0, 1, cameraData.GetAddressOf());
}