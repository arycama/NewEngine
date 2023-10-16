#pragma once

class GraphicsContext;
class Handle;
enum class TextureFormat;

class GraphicsDevice
{
public:
	virtual ~GraphicsDevice();

	virtual void ClearRenderTarget(float red, float green, float blue, float alpha) const = 0;

	virtual void EndScene() const = 0;

	virtual float GetAspectRatio() const = 0;

	virtual Handle CreateTexture2D(int width, int height) = 0;

	virtual Handle CreateShaderResourceView(const Handle& resource, TextureFormat format) = 0;

	virtual Handle CreateVertexShader(const void* shaderBytecode, int size) = 0;

	virtual Handle CreatePixelShader(const void* bytecode, int size) = 0;

	virtual Handle CreateInputLayout(const struct D3D11_INPUT_ELEMENT_DESC* inputs, int count, void* shader, int shaderSize) = 0;

	virtual Handle CreateSamplerState(struct CD3D11_SAMPLER_DESC& desc) = 0;

	virtual Handle CreateBuffer(const struct CD3D11_BUFFER_DESC& desc, const struct D3D11_SUBRESOURCE_DATA* initialData) = 0;

	virtual void ReleaseBuffer(const Handle& handle) = 0;

	// Todo: Move to system?
	virtual GraphicsContext& GetGraphicsContext() const = 0;
};