#pragma once

#include "Component.h"

class Model; 
class Shader;
class Transform;
class Graphics;
class Engine;

class Renderer : public Component
{
public:
	Renderer(Model& model, const Shader& shader, Transform& transform, const Graphics& graphics, Engine& engine, const Texture& texture);
	~Renderer();

	void Render(const Camera& camera) const;

private:
	Engine& engine;
	const Graphics& graphics;
	const Model& model;
	const Shader& shader;
	const Texture& texture;
	const Transform& transform;
};