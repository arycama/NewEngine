#pragma once

#include "Component.h"

class Renderer : public Component
{
public:
	Renderer(class Model& model, class Shader& shader, class Graphics& graphics, class Engine& engine);
	~Renderer();
	void Render(class Camera& camera);

private:
	class Engine& engine;
	class Graphics& graphics;
	class Model& model;
	class Shader& shader;
};