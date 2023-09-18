#pragma once

class GraphicsContext;
enum class TextureFormat;
struct ID3D11Device;

class GraphicsDevice
{
public:
	virtual ~GraphicsDevice();

	virtual void BeginScene(float red, float green, float blue, float alpha) const = 0;
	virtual void EndScene() const = 0;

	virtual float GetAspectRatio() const = 0;
	virtual void CreateTexture2D(int width, int height, struct ID3D11Texture2D** texture) = 0;
	virtual void CreateShaderResourceView(struct ID3D11Resource& resource, TextureFormat format, struct ID3D11ShaderResourceView** result) = 0;

	// Todo: abstract
	virtual ID3D11Device& GetDevice() const = 0;

	// Todo: Move to system
	virtual GraphicsContext& GetGraphicsContext() const = 0;
};