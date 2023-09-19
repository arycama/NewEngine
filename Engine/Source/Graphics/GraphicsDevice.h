#pragma once

class GraphicsContext;
enum class TextureFormat;

class GraphicsDevice
{
public:
	virtual ~GraphicsDevice();

	virtual void BeginScene(float red, float green, float blue, float alpha) const = 0;
	virtual void EndScene() const = 0;

	virtual float GetAspectRatio() const = 0;
	virtual void CreateTexture2D(int width, int height, struct ID3D11Texture2D** texture) = 0;
	virtual void CreateShaderResourceView(struct ID3D11Resource& resource, TextureFormat format, struct ID3D11ShaderResourceView** result) = 0;

	virtual void CreateVertexShader(const void* shaderBytecode, int size, struct ID3D11VertexShader** vertexShader) = 0;

	virtual void CreateInputLayout(const struct D3D11_INPUT_ELEMENT_DESC* inputs, int count);

	// Todo: Move to system
	virtual GraphicsContext& GetGraphicsContext() const = 0;
};