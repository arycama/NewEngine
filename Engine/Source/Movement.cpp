#include "Behaviour.h"
#include "Movement.h"
#include "Input.h"
#include "Int2.h"
#include "Transform.h"

#include <DirectXMath.h>
#include <DirectXMathConvert.inl>
#include <DirectXMathMisc.inl>
#include <DirectXMathVector.inl>

using namespace DirectX;
using namespace std;

Movement::Movement(const Input& input, Transform& transform, Engine& engine) : Behaviour(engine), input(input), transform(transform) { }

void Movement::Serialize(ofstream& stream) const
{
}

void Movement::Update()
{
	constexpr float rotateSpeed = XMConvertToRadians(0.010f);

	auto mouseDelta = input.GetMouseDelta();

	// Rotation
	auto rotation = transform.GetRotation();
	rotation.y += mouseDelta.x * rotateSpeed;
	rotation.x += mouseDelta.y * rotateSpeed;

	transform.SetRotation(rotation);

	auto movementSpeed = 0.1f;
	auto movement = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if (input.IsKeyDown(0x57)) // W
		movement.z += movementSpeed;

	if (input.IsKeyDown(0x53)) // S
		movement.z -= movementSpeed;

	if (input.IsKeyDown(0x41)) // A
		movement.x -= movementSpeed;

	if (input.IsKeyDown(0x44)) // D
		movement.x += movementSpeed;

	// Rotate the movement into the camera's coordinates
	auto rotationVector = XMLoadFloat3(&rotation);
	auto rotationQuaternion = XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	auto movementVector = XMLoadFloat3(&movement);
	auto rotatedMovement = XMVector3Rotate(movementVector, rotationQuaternion);

	auto position = transform.GetPosition();
	auto positionVector = XMLoadFloat3(&position);
	auto finalPosition = XMVectorAdd(positionVector, rotatedMovement);

	XMFLOAT3 result;
	XMStoreFloat3(&result, finalPosition);

	transform.SetPosition(result);
}
