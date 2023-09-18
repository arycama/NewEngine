#pragma once

class GraphicsContext;
struct ID3D11Device;
struct ID3D11DeviceContext;

class GraphicsDevice
{
public:
	virtual ~GraphicsDevice();

	virtual void BeginScene(float red, float green, float blue, float alpha) const = 0;
	virtual void EndScene() const = 0;

	virtual float GetAspectRatio() const = 0;

	// Todo: abstract
	virtual ID3D11Device& GetDevice() const = 0;
	virtual GraphicsContext& GetGraphicsContext() const = 0;
};