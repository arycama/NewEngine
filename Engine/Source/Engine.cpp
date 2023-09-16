#include "Behaviour.h"
#include "Camera.h"
#include "Engine.h"
#include "Entity.h"
#include "Graphics.h"
#include "Input.h"
#include "Int2.h"
#include "Material.h"
#include "Model.h"
#include "Movement.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Shader.h"
#include "System.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "Transform.h"
#include "WindowHandle.h"

using namespace std;
using namespace DirectX;

const bool fullScreen = false;

Engine::Engine(System& system) : isBeingUnloaded(false), system(system)
{
	int windowWidth, windowHeight;
	auto hwnd = system.InitializeWindow(fullScreen, windowWidth, windowHeight);

	windowHandle = make_unique<WindowHandle>(hwnd);
	graphics = make_unique<Graphics>(windowWidth, windowHeight, true, hwnd, fullScreen);
	input = make_unique<Input>();

	textureLoader = make_unique<TextureLoader>();
	resourceManager = make_unique<ResourceManager>(*textureLoader.get(), graphics->GetDevice(), graphics->GetDeviceContext());

	auto material = resourceManager->LoadMaterial("Assets/Rock.material");

	// Create the scene
	//auto& scene = *new Scene(*this);

	auto& scene = *new Scene("Assets/Scene.scene", *this, *resourceManager.get(), *graphics.get(), *input.get());
	AddScene(scene);

	/*new Entity("Assets/Camera.prefab", scene, *resourceManager.get(), *this, graphics->GetDevice(), graphics->GetDeviceContext(), *graphics.get(), *input.get());
	new Entity("Assets/Rock.prefab", scene, *resourceManager.get(), *this, graphics->GetDevice(), graphics->GetDeviceContext(), *graphics.get(), *input.get());
	new Entity("Assets/Cube.prefab", scene, *resourceManager.get(), *this, graphics->GetDevice(), graphics->GetDeviceContext(), *graphics.get(), *input.get());*/

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

	input->Reset();

	// Clear the buffers to begin the scene.
	graphics->BeginScene(0.0f, 0.5f, 1.0f, 1.0f);

	// Render the model using the texture shader.
	for (auto camera : cameras)
	{
		camera->Render();

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

void Engine::SetMousePosition(int xPos, int yPos)
{
	input->SetMousePosition(Int2(xPos, yPos));
}

void Engine::SetMouseDelta(int xDelta, int yDelta)
{
	input->SetMouseDelta(Int2(xDelta, yDelta));
}

void Engine::AddBehaviour(Behaviour& behaviour)
{
	behaviours.push_back(&behaviour);
}

void Engine::RemoveBehaviour(const Behaviour& behaviour)
{
	if(!isBeingUnloaded)
		behaviours.erase(find(behaviours.begin(), behaviours.end(), &behaviour));
}

void Engine::AddCamera(const Camera& camera)
{
	cameras.push_back(&camera);
}

void Engine::RemoveCamera(const Camera& camera)
{
	if (!isBeingUnloaded)
		cameras.erase(find(cameras.begin(), cameras.end(), &camera));
}

void Engine::AddRenderer(const Renderer& renderer)
{
	renderers.push_back(&renderer);
}

void Engine::RemoveRenderer(const Renderer& renderer)
{
	if(!isBeingUnloaded)
		renderers.erase(find(renderers.begin(), renderers.end(), &renderer));
}

void Engine::AddScene(const Scene& scene)
{
	scenes.push_back(unique_ptr<const Scene>(&scene));
}

void Engine::RemoveScene(const Scene& scene)
{
	if (isBeingUnloaded)
		return;

	// If we're not unloading, then this should only be getting called from Scene's destructor, so just release the pointer
	auto result = find_if(scenes.begin(), scenes.end(), [&](auto& obj) { return obj.get() == &scene; });
	result->release();
	scenes.erase(result);
}