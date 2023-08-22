#pragma once

#include "Component.h"

class Engine;
class Graphics;
class Model;
class Shader;

class Renderer : public Component
{
public:
	Renderer(Model& model, const Shader& shader, const Graphics& graphics, Engine& engine, const Texture& texture);
	~Renderer();

	void Render(const Camera& camera) const;

private:
	Engine& engine;
	const Graphics& graphics;
	const Model& model;
	const Shader& shader;
	const Texture& texture;
};