#pragma once

#include <wrl/client.h>

namespace DirectX
{
	struct XMMATRIX;
}

class RenderController
{
public:
	RenderController(struct ID3D11Device& device, struct ID3D11DeviceContext& deviceContext);

	void UpdateCameraData(const class Camera& camera);

	struct ID3D11Buffer& GetPerCameraData() const;

private:
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraData;
};