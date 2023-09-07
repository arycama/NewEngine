#pragma once

#include "Component.h"
#include <directxmath.h>

class Transform : public Component
{
public:
	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);

	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetRotation() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotation(DirectX::XMFLOAT3 rotation);

private:
	DirectX::XMFLOAT3 position, rotation;
};
