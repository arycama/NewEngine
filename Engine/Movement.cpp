#include "Movement.h"
#include "Input.h"
#include "Transform.h"

Movement::Movement(Input& input, Transform& transform) : input(input), transform(transform) { }

void Movement::Update()
{
	float movementSpeed = 0.1f;
	float rotateSpeed = 1.0f;

	auto position = transform.GetPosition();

	if (input.IsKeyDown(0x57)) // W
		position.z += movementSpeed;

	if (input.IsKeyDown(0x53)) // S
		position.z -= movementSpeed;

	if (input.IsKeyDown(0x41)) // A
		position.x -= movementSpeed;

	if (input.IsKeyDown(0x44)) // D
		position.x += movementSpeed;

	transform.SetPosition(position);

	// Rotation
	auto rotation = transform.GetRotation();
	if (input.IsKeyDown(0x25)) // Left
		rotation.y -= rotateSpeed;

	if (input.IsKeyDown(0x27)) // Right
		rotation.y += rotateSpeed;

	if (input.IsKeyDown(0x26)) // Up
		rotation.x -= rotateSpeed;

	if (input.IsKeyDown(0x28)) // Down
		rotation.x += rotateSpeed;

	transform.SetRotation(rotation);
}
