#pragma once

#include <string>
#include <wrl/client.h>

class Graphics;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

class Shader
{
public:
	Shader(const std::string& path, Graphics& graphics);
	void Render() const;

private:
	Graphics& graphics;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
};
