#include "Camera.h"
#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "Transform.h"

#include <directxmath.h>

using namespace DirectX;

Camera::Camera(Transform& transform, const float nearClipPlane, const float farClipPlane, const float fieldOfView, Graphics& graphics, Engine& engine) : transform(transform), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane), fieldOfView(fieldOfView), graphics(graphics), engine(engine)
{ 
	engine.AddCamera(*this);
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
	auto forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	auto lookAtVector = XMLoadFloat3(&forward);
	lookAtVector = XMVector3Rotate(lookAtVector, quaternionVector);

	auto up = XMFLOAT3(0.0f, 1.0f, 0.0f);
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
