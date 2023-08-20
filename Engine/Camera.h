#pragma once

#include "Component.h"

namespace DirectX
{
	struct XMMATRIX;
}

class Camera : public Component
{
public:
	Camera(class Transform& transform, const float nearClipPlane, const float farClipPlane, const float fieldOfView, class Renderer& renderer, class Engine& engine);
	//~Camera();

	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

private:
	float nearClipPlane, farClipPlane, fieldOfView;

	Engine& engine;
	Renderer& renderer;
	Transform& transform;
};
