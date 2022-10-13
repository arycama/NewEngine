#include "Camera.h"

using namespace DirectX;

Camera::Camera(XMFLOAT3 position, XMFLOAT3 rotation)
{
	this->position = position;
	this->rotation = rotation;
}

XMMATRIX& Camera::GetViewMatrix()
{
	// Setup the vector that points upwards.
	auto up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// Load it into a XMVECTOR structure.
	auto upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world and load it into a XMVECTOR structure.
	auto positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	auto lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);

	// Load it into a XMVECTOR structure.
	auto lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	auto pitch = XMConvertToRadians(rotation.x);
	auto yaw = XMConvertToRadians(rotation.y);
	auto roll = XMConvertToRadians(rotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	auto viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return viewMatrix;
}