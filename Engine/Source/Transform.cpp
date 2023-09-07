#include "Transform.h"
#include <DirectXMath.h>
#include <DirectXMathConvert.inl>
#include <DirectXMathMatrix.inl>
#include <DirectXMathMisc.inl>

using namespace DirectX;

Transform::Transform(XMFLOAT3 position, XMFLOAT3 rotation) : position(position), rotation(rotation) { }

XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

XMFLOAT3 Transform::GetRotation() const
{
	return rotation;
}

XMMATRIX Transform::GetWorldMatrix() const
{
	auto positionVector = XMLoadFloat3(&position);

	auto rotationOrigin = XMFLOAT3(0.0f, 0.0f, 0.0f);
	auto rotationOriginVector = XMLoadFloat3(&rotationOrigin);

	auto rotationVector = XMLoadFloat3(&rotation);
	auto rotation = XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	auto scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	auto scaleVector = XMLoadFloat3(&scale);

	return XMMatrixAffineTransformation(scaleVector, rotationOriginVector, rotation, positionVector);
}

void Transform::SetPosition(const XMFLOAT3 position)
{
	this->position = position;
}

void Transform::SetRotation(const XMFLOAT3 rotation)
{
	this->rotation = rotation;
}
