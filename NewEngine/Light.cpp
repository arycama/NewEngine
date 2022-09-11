#include "Light.h"

using namespace DirectX;

Light::Light(XMFLOAT3 direction, XMFLOAT3 color)
{
	this->direction = direction;
	this->color = color;
}

XMFLOAT3 Light::GetDirection()
{
	return direction;
}

XMFLOAT3 Light::GetColor()
{
	return color;
}
