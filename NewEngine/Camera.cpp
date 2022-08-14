#include "Camera.h"

Camera::Camera(D3DXVECTOR3 position, D3DXVECTOR3 rotation)
{
	this->position = position;
	this->rotation = rotation;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return position;
}

D3DXVECTOR3 Camera::GetRotation()
{
	return rotation;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}

void Camera::Render()
{
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	auto pitch = D3DXToRadian(rotation.x);
	auto yaw = D3DXToRadian(rotation.y);
	auto roll = D3DXToRadian(rotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Setup where the camera is looking by default.
	auto lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);

	// Setup the vector that points upwards.
	auto up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}

