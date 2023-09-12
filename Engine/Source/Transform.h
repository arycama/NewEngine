#pragma once

#include "Component.h"
#include <directxmath.h>
#include <fstream>
#include <string>

class Transform : public Component
{
public:
	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);

	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetRotation() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotation(DirectX::XMFLOAT3 rotation);

	void Serialize(std::ofstream& stream) const override;

private:
	DirectX::XMFLOAT3 position, rotation;
};
