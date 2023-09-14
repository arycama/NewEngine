#pragma once

#include "Component.h"
#include <fstream>
#include <string>
#include <wrl/client.h>

class Engine;
class Entity;
class Material;
class Model;
class Transform;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Renderer : public Component
{
public:
	Renderer(std::shared_ptr<const Model> model, std::shared_ptr<const Material> material, const Transform& transform, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& deviceContext, const Entity& entity);

	Renderer(std::istream& stream);

	~Renderer();

	void Serialize(std::ostream& stream) const override;
	void Render() const;

private:
	const Entity& entity;
	const Transform& transform;
	const std::shared_ptr<const Model> model;
	const std::shared_ptr<const Material> material;

	Engine& engine;
	ID3D11DeviceContext& deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> drawData;
};