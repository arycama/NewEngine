#include "Transform.h"
#include <DirectXMath.h>
#include <DirectXMathConvert.inl>
#include <DirectXMathMatrix.inl>
#include <DirectXMathMisc.inl>

using namespace DirectX;
using namespace std;

Transform::Transform(XMFLOAT3 position, XMFLOAT3 rotation) : position(position), rotation(rotation) { }

Transform::Transform(istream& stream)
{
	stream >> position.x >> position.y >> position.z;
	stream >> rotation.x >> rotation.y >> rotation.z;
}

void Transform::Serialize(ostream& stream) const
{
	stream << "transform" << ' ';
	stream << position.x << ' ' << position.y << ' ' << position.z << ' ';
	stream << rotation.x << ' ' << rotation.y << ' ' << rotation.z << ' ';
}

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

	XMFLOAT3 rotationOrigin(0.0f, 0.0f, 0.0f);
	auto rotationOriginVector = XMLoadFloat3(&rotationOrigin);

	auto rotationVector = XMLoadFloat3(&rotation);
	auto rotation = XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	XMFLOAT3 scale(1.0f, 1.0f, 1.0f);
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