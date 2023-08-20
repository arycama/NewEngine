#include "Engine.h"

#include "Camera.h"
#include "Renderer.h"
#include "Input.h"
#include "Model.h"
#include "System.h"
#include "TextureShader.h"
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

	renderer = make_unique<Renderer>(width, height, VSYNC_ENABLED, hwnd, fullScreen);

	input = make_unique<Input>();

	// Create the camera object and set the initial position of the camera.
	auto cameraPosition = XMFLOAT3(0.0f, 0.0f, -5.0f);
	auto cameraRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	camera = make_unique<Camera>(cameraPosition, cameraRotation, 0.1f, 1000.0f, 45, *renderer, *input);

	// Create and initialize the model object.
	model = make_unique<Model>(renderer->GetDevice(), renderer->GetDeviceContext(), "../Engine/data/stone01.tga");

	// Create and initialize the texture shader object.
	textureShader = make_unique<TextureShader>(renderer->GetDevice());

	// Hide the mouse cursor.
	ShowCursor(false);
}

Engine::~Engine()
{
	system.ReleaseWindow(windowHandle->GetHandle(), fullScreen);

	// Show the mouse cursor.
	ShowCursor(true);
}

void Engine::Update() const
{
	// Update scene
	camera->Update();

	// Clear the buffers to begin the scene.
	renderer->BeginScene(0.0f, 0.5f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	// Get the world, view, and projection matrices from the camera and renderer objects.

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(renderer->GetDeviceContext());

	// Render the model using the texture shader.
	textureShader->Render(renderer->GetDeviceContext(), model->GetIndexCount(), camera->GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), model->GetTexture());

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
