#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include <wrl/client.h>

class Shader
{
public:
	Shader(ID3D11Device& device);

	bool Render(ID3D11DeviceContext&, int, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView&);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState;

	bool SetShaderParameters(ID3D11DeviceContext&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView&) const;
	void RenderShader(ID3D11DeviceContext&, int) const;

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
};
