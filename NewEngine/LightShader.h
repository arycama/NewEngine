#pragma once

#include <D3D11.h>
#include <DirectXMath.h>

class LightShader
{
public:
	LightShader(ID3D11Device*, HWND);
	~LightShader();

	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT3);

private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct LightBuffer
	{
		DirectX::XMFLOAT3 diffuseColor;
		float padding0;
		DirectX::XMFLOAT3 lightDirection;
		float padding1;
	};

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* samplerState;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
};
