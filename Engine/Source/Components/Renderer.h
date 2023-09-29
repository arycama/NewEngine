#pragma once

#include "Component.h"
#include "Handle.h"

#include <fstream>
#include <string>
#include <wrl/client.h>

class Engine;
class Entity;
class GraphicsContext;
class GraphicsDevice;
class Material;
class Model;
class ResourceManager;
class Transform;

struct ID3D11Buffer;

class Renderer : public Component
{
public:
	Renderer(std::shared_ptr<const Model> model, std::shared_ptr<const Material> material, const Transform& transform, Engine& engine, GraphicsDevice& graphicsDevice, const Entity& entity);

	Renderer(std::istream& stream, ResourceManager& resourceManager, Engine& engine, GraphicsDevice& graphicsDevice, const Entity& entity);

	~Renderer();

	void Serialize(std::ostream& stream) const override;
	void Render(GraphicsContext& graphicsContext) const;

private:
	GraphicsDevice& graphicsDevice;
	const Entity& entity;
	const Transform* transform;
	std::shared_ptr<const Model> model;
	std::shared_ptr<const Material> material;

	Engine& engine;
	Handle drawData;
};