#include "Camera.h"
#include "Engine.h"
#include "Entity.h"
#include "GraphicsContext.h"
#include "GraphicsDevice.h"
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
using namespace std;

struct PerCameraData
{
	XMMATRIX view;
	XMMATRIX projection;
};

Camera::Camera(float nearClipPlane, float farClipPlane, float fieldOfView, const Transform& transform, GraphicsDevice& graphicsDevice, Engine& engine, Entity& entity) : transform(&transform), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane), fieldOfView(fieldOfView), graphicsDevice(graphicsDevice), engine(engine), entity(entity)
{ 
	engine.AddCamera(*this);
	CD3D11_BUFFER_DESC cameraDataDesc(sizeof(PerCameraData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	graphicsDevice.CreateBuffer(cameraDataDesc, nullptr, cameraData.GetAddressOf());
	SetDebugObjectName(cameraData.Get(), "Camera Data");
}

Camera::Camera(std::istream& stream, GraphicsDevice& graphicsDevice, Engine& engine, Entity& entity) : graphicsDevice(graphicsDevice), engine(engine), entity(entity)
{
	int transformIndex;
	stream >> transformIndex;
	transform = &dynamic_cast<Transform&>(entity.GetComponentAt(transformIndex));

	stream >> nearClipPlane;
	stream >> farClipPlane;
	stream >> fieldOfView;

	engine.AddCamera(*this);
	CD3D11_BUFFER_DESC cameraDataDesc(sizeof(PerCameraData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	graphicsDevice.CreateBuffer(cameraDataDesc, nullptr, cameraData.GetAddressOf());
	SetDebugObjectName(cameraData.Get(), "Camera Data");
}

Camera::~Camera()
{
	engine.RemoveCamera(*this);
}

void Camera::Serialize(ostream& stream) const
{
	stream << "camera" << ' ';
	stream << entity.GetComponentIndex(*transform) << ' ';
	stream << nearClipPlane << ' ' << farClipPlane << ' ' << fieldOfView;
}

XMMATRIX Camera::GetViewMatrix() const
{
	// Setup the position of the camera in the world and load it into a XMVECTOR structure.
	auto position = transform->GetPosition();
	auto positionVector = XMLoadFloat3(&position);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	auto rotation = transform->GetRotation();
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
	const auto aspect = graphicsDevice.GetAspectRatio();
	const auto fovRadians = XMConvertToRadians(fieldOfView);
	return XMMatrixPerspectiveFovLH(fovRadians, aspect, nearClipPlane, farClipPlane);
}

void Camera::Render(GraphicsContext& graphicsContext) const
{
	D3D11_MAPPED_SUBRESOURCE perCameraDataMappedResource;
	graphicsContext.BeginWrite(cameraData.Get(), &perCameraDataMappedResource);

	auto perCameraDataPtr = static_cast<PerCameraData*>(perCameraDataMappedResource.pData);

	// Copy the matrices into the constant buffer.
	perCameraDataPtr->view = GetViewMatrix();
	perCameraDataPtr->projection = GetProjectionMatrix();

	graphicsContext.EndWrite(cameraData.Get());
	graphicsContext.VSSetConstantBuffers(0, 1, cameraData.GetAddressOf());
}