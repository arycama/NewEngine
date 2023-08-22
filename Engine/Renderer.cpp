#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Model.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

Renderer::Renderer(Model& model, Shader& shader, Transform& transform, Graphics& graphics, Engine& engine, Texture& texture) : model(model), shader(shader), transform(transform), graphics(graphics), engine(engine), texture(texture)
{
	engine.AddRenderer(*this);
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Render(Camera& camera)
{
	shader.Render(graphics.GetDeviceContext(), transform.GetWorldMatrix(), camera.GetViewMatrix(), camera.GetProjectionMatrix(), texture.GetTexture());
	model.Render();
}
