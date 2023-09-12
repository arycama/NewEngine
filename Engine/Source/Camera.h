#pragma once

#include "Component.h"
#include <string>
#include <wrl/client.h>

class Engine; 
class Graphics; 
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
	Camera(float nearClipPlane, float farClipPlane, float fieldOfView, const Transform& transform, const Graphics& graphics, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext);
	~Camera();

	void Serialize(std::ofstream& stream) const override;

	void Render() const;

private:
	const float nearClipPlane, farClipPlane, fieldOfView;

	const Graphics& graphics;
	const Transform& transform;
	Engine& engine;
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraData;

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;
};