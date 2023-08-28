#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>

class Texture
{
public:
	Texture(const std::string& filename, ID3D11Device& device, ID3D11DeviceContext& deviceContext);

	ID3D11ShaderResourceView& GetTexture() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
};
