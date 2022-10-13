#pragma once

#include <directxmath.h>

class Camera
{
public:
	Camera(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	DirectX::XMMATRIX& GetViewMatrix();

private:
	DirectX::XMFLOAT3 position, rotation;
};
