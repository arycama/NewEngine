#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "Input.h"

class Camera
{
public:
	Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Input* input);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMMATRIX GetViewMatrix();

	void Update();

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;

	Input* input;
};
