#pragma once

#include <directxmath.h>

class Input;
class Renderer;

class Camera
{
public:
	Camera(const DirectX::XMFLOAT3 position, const DirectX::XMFLOAT3 rotation, const float nearClipPlane, const float farClipPlane, const float fieldOfView, Renderer& renderer, Input& input);

	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

	void Update();

private:
	float nearClipPlane, farClipPlane, fieldOfView;
	DirectX::XMFLOAT3 position, rotation;

	Input& input;
	Renderer& renderer;
};
