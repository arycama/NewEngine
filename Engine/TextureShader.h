#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

class TextureShader
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	TextureShader(ID3D11Device*);
	~TextureShader();

	bool Render(ID3D11DeviceContext*, int, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const  DirectX::XMMATRIX&, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView*) const;
	void RenderShader(ID3D11DeviceContext*, int) const;

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11SamplerState* m_sampleState;
};
