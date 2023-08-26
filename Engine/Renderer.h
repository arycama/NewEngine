#pragma once

#include "Component.h"
#include <wrl/client.h>

class Engine;
class Graphics;
class Material;
class Model;
class Transform;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Renderer : public Component
{
public:
	Renderer(const Model& model, const Material& material, const Transform& transform, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext);
	~Renderer();

	void Render() const;

private:
	const Transform& transform;
	const Model& model;
	const Material& material;

	Engine& engine;
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> drawData;

	void UpdateDrawData() const;
};