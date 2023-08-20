#include "Camera.h"
#include "Input.h"
#include "Renderer.h"

using namespace DirectX;

Camera::Camera(const XMFLOAT3 position, const XMFLOAT3 rotation, const float nearClipPlane, const float farClipPlane, const float fieldOfView, Renderer& renderer, Input& input) : position(position), rotation(rotation), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane), fieldOfView(fieldOfView), renderer(renderer), input(input)
{
}

XMMATRIX Camera::GetWorldMatrix() const
{
	return XMMatrixIdentity();
}

XMMATRIX Camera::GetViewMatrix() const
{
	// Setup the vector that points upwards.
	constexpr auto up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// Load it into a XMVECTOR structure.
	auto upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world and load it into a XMVECTOR structure.
	const auto positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	constexpr auto lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);

	// Load it into a XMVECTOR structure.
	auto lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	const auto pitch = XMConvertToRadians(rotation.x);
	const auto yaw = XMConvertToRadians(rotation.y);
	const auto roll = XMConvertToRadians(rotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	const auto rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	auto viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return viewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	const auto aspect = renderer.GetAspectRatio();
	const auto fovRadians = XMConvertToRadians(fieldOfView);
	return XMMatrixPerspectiveFovLH(fovRadians, aspect, nearClipPlane, farClipPlane);
}

void Camera::Update()
{
	float movementSpeed = 0.1f;
	float rotateSpeed = 1.0f;

	if (input.IsKeyDown(0x57)) // W
		position.z += movementSpeed;

	if (input.IsKeyDown(0x53)) // S
		position.z -= movementSpeed;

	if (input.IsKeyDown(0x41)) // A
		position.x -= movementSpeed;

	if (input.IsKeyDown(0x44)) // D
		position.x += movementSpeed;

	if (input.IsKeyDown(VK_LEFT)) // Left
		rotation.y -= rotateSpeed;

	if (input.IsKeyDown(VK_RIGHT)) // Right
		rotation.y += rotateSpeed;

	if (input.IsKeyDown(VK_UP)) // Up
		rotation.x -= rotateSpeed;

	if (input.IsKeyDown(VK_DOWN)) // Down
		rotation.x += rotateSpeed;
}