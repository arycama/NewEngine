#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Model.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

Renderer::Renderer(Model& model, Shader& shader, Graphics& graphics, Engine& engine, Texture& texture) : model(model), shader(shader), graphics(graphics), engine(engine), texture(texture)
{
	engine.AddRenderer(*this);
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Render(Camera& camera)
{
	model.Render(graphics.GetDeviceContext());
	shader.Render(graphics.GetDeviceContext(), model.GetIndexCount(), camera.GetWorldMatrix(), camera.GetViewMatrix(), camera.GetProjectionMatrix(), texture.GetTexture());
}
