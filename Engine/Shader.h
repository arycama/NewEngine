#pragma once

#include <wrl/client.h>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11VertexShader;

namespace DirectX
{
	struct XMMATRIX;
}

class Shader
{
public:
	Shader(ID3D11Device& device);
	void Render(ID3D11DeviceContext& deviceContext, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView& texture) const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};
