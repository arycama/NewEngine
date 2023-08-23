#pragma once

#include "Component.h"

class Engine;
class Graphics;
class Model;
class RenderController;
class Shader;
class Transform;

class Renderer : public Component
{
public:
	Renderer(Model& model, const Shader& shader, const Graphics& graphics, Engine& engine, const Texture& texture, Transform& transform, RenderController& renderController);
	~Renderer();

	void Render() const;

private:
	Engine& engine;
	Transform& transform;
	RenderController& renderController;
	const Graphics& graphics;
	const Model& model;
	const Shader& shader;
	const Texture& texture;
};