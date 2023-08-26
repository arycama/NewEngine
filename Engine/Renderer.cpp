#include "Camera.h"
#include "Engine.h"
#include "Graphics.h"
#include "Material.h"
#include "Model.h"
#include "RenderController.h"
#include "Renderer.h"
#include "Transform.h"

#include <comdef.h>
#include <DirectXMath.h>

using namespace _com_util;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

Renderer::Renderer(Model& model, const Material& material, const Graphics& graphics, Engine& engine, Transform& transform, RenderController& renderController) : model(model), material(material), graphics(graphics), engine(engine), transform(transform), renderController(renderController)
{
	engine.AddRenderer(*this);
}

Renderer::~Renderer()
{
	engine.RemoveRenderer(*this);
}

void Renderer::Render() const
{
	renderController.UpdateDrawData(transform.GetWorldMatrix());
	material.Render();
	model.Render();
}
