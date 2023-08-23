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
	void UpdateDrawData(DirectX::XMMATRIX model);

	struct ID3D11Buffer& GetPerCameraData() const;
	struct ID3D11Buffer& GetPerDrawData() const;

private:
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> drawData;
};