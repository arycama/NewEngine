#include "Behaviour.h"
#include "Camera.h"
#include "Engine.h"
#include "Entity.h"
#include "Graphics.h"
#include "Input.h"
#include "Material.h"
#include "Model.h"
#include "Movement.h"
#include "RenderController.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "System.h"
#include "Texture.h"
#include "Transform.h"
#include "WindowHandle.h"

#include <string>

using namespace std;
using namespace DirectX;

const bool fullScreen = false;

Engine::Engine(System& system) : isBeingUnloaded(false), system(system)
{
	int width, height;
	auto hwnd = system.InitializeWindow(fullScreen, width, height);

	windowHandle = make_unique<WindowHandle>(hwnd);
	graphics = make_unique<Graphics>(width, height, true, hwnd, fullScreen);
	input = make_unique<Input>();
	renderController = make_unique<RenderController>(graphics->GetDevice(), graphics->GetDeviceContext());

	// Assets
	shader = make_unique<Shader>(graphics->GetDevice(), graphics->GetDeviceContext(), renderController->GetPerCameraData(), renderController->GetPerDrawData());
	texture = make_unique<Texture>(graphics->GetDevice(), graphics->GetDeviceContext(), "../Engine/data/stone01.tga");
	material = make_unique<Material>(*shader, *texture);

	// Create the scene
	auto scene = new Scene(*this);
	AddScene(*scene);

	// Create the camera object and set the initial position of the camera.
	auto& camera = *new Entity("Camera", *scene);

	auto& cameraTransform = camera.AddComponent<Transform>(XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	camera.AddComponent<Camera>(0.1f, 1000.0f, 45, cameraTransform, *graphics, *this);
	camera.AddComponent<Movement>(*input.get(), cameraTransform, *this);

	{
		auto& object = *new Entity("Model 0", *scene);
		auto& modelTransform = object.AddComponent<Transform>(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		auto& model = object.AddComponent<Model>(graphics->GetDevice(), graphics->GetDeviceContext(), modelTransform);
		object.AddComponent<Renderer>(model, *material.get(), *graphics, *this, modelTransform, *renderController);
	}

	{
		auto& object = *new Entity("Model 1", *scene);
		auto& modelTransform = object.AddComponent<Transform>(XMFLOAT3(2.5f, 0.0f, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		auto& model = object.AddComponent<Model>(graphics->GetDevice(), graphics->GetDeviceContext(), modelTransform);
		object.AddComponent<Renderer>(model, *material.get(), *graphics, *this, modelTransform, *renderController);
	}

	{
		auto& object = *new Entity("Model 2", *scene);
		auto& modelTransform = object.AddComponent<Transform>(XMFLOAT3(-2.5f, 0.0f, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		auto& model = object.AddComponent<Model>(graphics->GetDevice(), graphics->GetDeviceContext(), modelTransform);
		object.AddComponent<Renderer>(model, *material.get(), *graphics, *this, modelTransform, *renderController);
	}

	// Hide the mouse cursor.
	ShowCursor(false);
}

Engine::~Engine()
{
	system.ReleaseWindow(windowHandle->GetHandle(), fullScreen);
	ShowCursor(true);
}

void Engine::Update()
{
	// Update scene
	for (auto behaviour : behaviours)
		behaviour->Update();

	// Clear the buffers to begin the scene.
	graphics->BeginScene(0.0f, 0.5f, 1.0f, 1.0f);

	// Render the model using the texture shader.
	for (auto camera : cameras)
	{
		renderController->UpdateCameraData(*camera);

		for (auto renderer : renderers)
		{
			renderer->Render();
		}
	}

	// Present the rendered scene to the screen.
	graphics->EndScene();
}

void Engine::KeyDown(int key)
{
	if (key == VK_ESCAPE)
	{
		system.Quit();
		return;
	}

	input->SetKeyDown(key);
}

void Engine::KeyUp(int key)
{
	input->SetKeyUp(key);
}

void Engine::AddBehaviour(Behaviour& behaviour)
{
	behaviours.push_back(&behaviour);
}

void Engine::RemoveBehaviour(Behaviour& behaviour)
{
	if(!isBeingUnloaded)
		behaviours.erase(find(behaviours.begin(), behaviours.end(), &behaviour));
}

void Engine::AddCamera(Camera& camera)
{
	cameras.push_back(&camera);
}

void Engine::RemoveCamera(Camera& camera)
{
	if (!isBeingUnloaded)
		cameras.erase(find(cameras.begin(), cameras.end(), &camera));
}

void Engine::AddRenderer(Renderer& renderer)
{
	renderers.push_back(&renderer);
}

void Engine::RemoveRenderer(Renderer& renderer)
{
	if(!isBeingUnloaded)
		renderers.erase(find(renderers.begin(), renderers.end(), &renderer));
}

void Engine::AddScene(Scene& scene)
{
	scenes.push_back(unique_ptr<Scene>(&scene));
}

void Engine::RemoveScene(Scene& scene)
{
	if (isBeingUnloaded)
		return;

	// If we're not unloading, then this should only be getting called from Scene's destructor, so just release the pointer
	auto result = find_if(scenes.begin(), scenes.end(), [&](auto& obj) { return obj.get() == &scene; });
	result->release();
	scenes.erase(result);
}