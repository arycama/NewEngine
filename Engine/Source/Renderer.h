#pragma once

#include "Component.h"
#include <wrl/client.h>

class Engine;
class Material;
class Model;
class Transform;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Renderer : public Component
{
public:
	Renderer(const Model& model, std::shared_ptr<const Material> material, const Transform& transform, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext);
	~Renderer();

	void Render() const;

private:
	const Transform& transform;
	const Model& model;
	const std::shared_ptr<const Material> material;

	Engine& engine;
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> drawData;
};