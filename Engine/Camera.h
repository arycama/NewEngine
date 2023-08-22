#pragma once

#include "Component.h"

namespace DirectX
{
	struct XMMATRIX;
}

class Engine; 
class Graphics; 
class Transform;

class Camera : public Component
{
public:
	Camera(float nearClipPlane, float farClipPlane, float fieldOfView, const Transform& transform, const Graphics& graphics, Engine& engine);
	~Camera();

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

private:
	float nearClipPlane, farClipPlane, fieldOfView;

	Engine& engine;
	const Graphics& graphics;
	const Transform& transform;
};
