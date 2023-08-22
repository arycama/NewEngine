#include "Behaviour.h"
#include "Camera.h"
#include "Engine.h"
#include "Entity.h"
#include "Graphics.h"
#include "Input.h"
#include "Model.h"
#include "Movement.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "System.h"
#include "Texture.h"
#include "Transform.h"
#include "WindowHandle.h"

using namespace std;
using namespace DirectX;

const bool fullScreen = false;
const bool VSYNC_ENABLED = true;

Engine::Engine(System& system) : system(system)
{
	int width, height;
	auto hwnd = system.InitializeWindow(fullScreen, width, height);

	windowHandle = make_unique<WindowHandle>(hwnd);
	graphics = make_unique<Graphics>(width, height, VSYNC_ENABLED, hwnd, fullScreen);
	input = make_unique<Input>();

	// Assets
	shader = make_unique<Shader>(graphics->GetDevice());
	texture = make_unique<Texture>(graphics->GetDevice(), graphics->GetDeviceContext(), "../Engine/data/stone01.tga");

	// Create the scene
	auto scene = new Scene(*this);
	AddScene(*scene);

	// Create the camera object and set the initial position of the camera.
	auto& camera = *new Entity(*scene);

	auto cameraPosition = XMFLOAT3(0.0f, 0.0f, -5.0f);
	auto cameraRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	auto cameraTransform = new Transform(cameraPosition, cameraRotation);
	camera.AddComponent(*cameraTransform);
	camera.AddComponent(*new Camera(0.1f, 1000.0f, 45, *cameraTransform, *graphics, *this));
	camera.AddComponent(*new Movement(*input.get(), *cameraTransform, *this));

	{
		auto& object = *new Entity(*scene);
		auto modelTransform = new Transform(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		auto model = new Model(graphics->GetDevice(), graphics->GetDeviceContext());
		object.AddComponent(*modelTransform);
		object.AddComponent(*model);
		object.AddComponent(*new Renderer(*model, *shader.get(), *modelTransform, *graphics, *this, *texture.get()));
	}

	{
		auto& object = *new Entity(*scene);
		auto modelTransform = new Transform(XMFLOAT3(2.5f, 0.0f, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		auto model = new Model(graphics->GetDevice(), graphics->GetDeviceContext());
		object.AddComponent(*modelTransform);
		object.AddComponent(*model);
		object.AddComponent(*new Renderer(*model, *shader.get(), *modelTransform, *graphics, *this, *texture.get()));
	}

	{
		auto& object = *new Entity(*scene);
		auto modelTransform = new Transform(XMFLOAT3(-2.5f, 0.0f, 2.5f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		auto model = new Model(graphics->GetDevice(), graphics->GetDeviceContext());
		object.AddComponent(*modelTransform);
		object.AddComponent(*model);
		object.AddComponent(*new Renderer(*model, *shader.get(), *modelTransform, *graphics, *this, *texture.get()));
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
		for (auto renderer : renderers)
			renderer->Render(*camera);

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
	behaviours.erase(find(behaviours.begin(), behaviours.end(), &behaviour));
}

void Engine::AddCamera(Camera& camera)
{
	cameras.push_back(&camera);
}

void Engine::RemoveCamera(Camera& camera)
{
	cameras.erase(find(cameras.begin(), cameras.end(), &camera));
}

void Engine::AddRenderer(Renderer& renderer)
{
	renderers.push_back(&renderer);
}

void Engine::RemoveRenderer(Renderer& renderer)
{
	renderers.erase(find(renderers.begin(), renderers.end(), &renderer));
}

void Engine::AddScene(Scene& scene)
{
	scenes.push_back(unique_ptr<Scene>(&scene));
}

void Engine::RemoveScene(Scene& scene)
{
	//scenes.erase(find(scenes.begin(), scenes.end(), &scene));
}