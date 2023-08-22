#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Model.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

Renderer::Renderer(Model& model, const Shader& shader, Transform& transform, const Graphics& graphics, Engine& engine, const Texture& texture) : model(model), shader(shader), transform(transform), graphics(graphics), engine(engine), texture(texture)
{
	engine.AddRenderer(*this);
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Render(const Camera& camera) const
{
	shader.Render(transform.GetWorldMatrix(), camera.GetViewMatrix(), camera.GetProjectionMatrix(), texture.GetTexture());
	model.Render();
}
