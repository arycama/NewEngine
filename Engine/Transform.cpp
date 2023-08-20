#include "Transform.h"

Transform::Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation) : position(position), rotation(rotation)
{
}

DirectX::XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetRotation() const
{
	return rotation;
}

void Transform::SetPosition(const DirectX::XMFLOAT3 position)
{
	this->position = position;
}

void Transform::SetRotation(const DirectX::XMFLOAT3 rotation)
{
	this->rotation = rotation;
}
