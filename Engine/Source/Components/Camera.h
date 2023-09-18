#pragma once

#include "Component.h"
#include <string>
#include <wrl/client.h>

class Engine; 
class Entity;
class GraphicsDevice; 
class Transform;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace DirectX
{
	struct XMMATRIX;
}

class Camera : public Component
{
public:
	Camera(float nearClipPlane, float farClipPlane, float fieldOfView, const Transform& transform, const GraphicsDevice& graphicsDevice, Engine& engine, Entity& entity);
	Camera(std::istream& stream, const GraphicsDevice& graphicsDevice, Engine& engine, Entity& entity);
	~Camera();

	void Serialize(std::ostream& stream) const override;

	void Render() const;

private:
	float nearClipPlane, farClipPlane, fieldOfView;

	const GraphicsDevice& graphicsDevice;
	const Transform* transform;
	Engine& engine;
	Entity& entity;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraData;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;
};