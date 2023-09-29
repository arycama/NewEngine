#pragma once

#include "Component.h"
#include "Handle.h"

#include <string>
#include <wrl/client.h>

class Engine; 
class Entity;
class GraphicsContext;
class GraphicsDevice;
class Transform;
struct ID3D11Buffer;

namespace DirectX
{
	struct XMMATRIX;
}

class Camera : public Component
{
public:
	Camera(float nearClipPlane, float farClipPlane, float fieldOfView, const Transform& transform, GraphicsDevice& graphicsDevice, Engine& engine, Entity& entity);
	Camera(std::istream& stream, GraphicsDevice& graphicsDevice, Engine& engine, Entity& entity);
	~Camera();

	void Serialize(std::ostream& stream) const override;

	void Render(GraphicsContext& graphicsContext) const;

private:
	float nearClipPlane, farClipPlane, fieldOfView;

	const GraphicsDevice& graphicsDevice;
	const Transform* transform;
	Engine& engine;
	Entity& entity;

	Handle cameraData;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;
};