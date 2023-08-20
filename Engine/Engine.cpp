#include "Engine.h"

#include "Behaviour.h"
#include "Camera.h"
#include "Entity.h"
#include "Renderer.h"
#include "Input.h"
#include "Model.h"
#include "Movement.h"
#include "System.h"
#include "TextureShader.h"
#include "Transform.h"
#include "WindowHandle.h"

#include <stdexcept>

using namespace std;
using namespace DirectX;

const bool fullScreen = false;
const bool VSYNC_ENABLED = true;

Engine::Engine(System& system) : system(system)
{
	int width, height;
	auto hwnd = system.InitializeWindow(fullScreen, width, height);

	windowHandle = make_unique<WindowHandle>(hwnd);
	renderer = make_unique<Renderer>(width, height, VSYNC_ENABLED, hwnd, fullScreen);
	input = make_unique<Input>();

	// Create the camera object and set the initial position of the camera.
	auto cameraEntity = new Entity(*this);

	auto cameraPosition = XMFLOAT3(0.0f, 0.0f, -5.0f);
	auto cameraRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	auto cameraTransform = new Transform(cameraPosition, cameraRotation);
	cameraEntity->AddComponent(cameraTransform);
	cameraEntity->AddComponent(new Camera(*cameraTransform, 0.1f, 1000.0f, 45, *renderer, *this));

	auto movement = new Movement(*this, *input.get(), *cameraTransform);
	cameraEntity->AddComponent(movement);

	// Create and initialize the model object.
	model = make_unique<Model>(renderer->GetDevice(), renderer->GetDeviceContext(), "../Engine/data/stone01.tga");

	// Create and initialize the texture shader object.
	textureShader = make_unique<TextureShader>(renderer->GetDevice());

	// Hide the mouse cursor.
	ShowCursor(false);
}

Engine::~Engine()
{
	// Delete all entities
	for (auto entity : entities)
	{
		delete entity;
	}

	system.ReleaseWindow(windowHandle->GetHandle(), fullScreen);

	// Show the mouse cursor.
	ShowCursor(true);
}

void Engine::Update()
{
	// Update scene
	for (auto behaviour : behaviours)
	{
		behaviour->Update();
	}

	// Clear the buffers to begin the scene.
	renderer->BeginScene(0.0f, 0.5f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	// Get the world, view, and projection matrices from the camera and renderer objects.

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(renderer->GetDeviceContext());

	// Render the model using the texture shader.
	for (auto camera : cameras)
	{
		textureShader->Render(renderer->GetDeviceContext(), model->GetIndexCount(), camera->GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), model->GetTexture());
	}

	// Present the rendered scene to the screen.
	renderer->EndScene();
}

void Engine::KeyDown(const unsigned int key)
{
	if (key == VK_ESCAPE)
	{
		system.Quit();
		return;
	}

	input->SetKeyDown(key);
}

void Engine::KeyUp(unsigned int key)
{
	input->SetKeyUp(key);
}

void Engine::AddEntity(Entity* entity)
{
	assert(entity);
	entities.push_back(entity);
}

void Engine::RemoveEntity(Entity* entity)
{
	assert(entity);
	entities.erase(remove(entities.begin(), entities.end(), entity), entities.end());
}

void Engine::AddCamera(Camera* camera)
{
	assert(camera);
	cameras.push_back(camera);
}

void Engine::RemoveCamera(Camera* camera)
{
	assert(camera);
	cameras.erase(remove(cameras.begin(), cameras.end(), camera), cameras.end());
}

void Engine::AddBehaviour(Behaviour* behaviour)
{
	assert(behaviour);
	behaviours.push_back(behaviour);
}

void Engine::RemoveBehaviour(Behaviour* behaviour)
{
	assert(behaviour);
	behaviours.erase(remove(behaviours.begin(), behaviours.end(), behaviour), behaviours.end());
}