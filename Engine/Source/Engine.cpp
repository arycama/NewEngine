#include "Behaviour.h"
#include "Camera.h"
#include "Engine.h"
#include "GraphicsDevice.h"
#include "Graphics/D3D11/D3D11GraphicsDevice.h" // Todo: Get this from System
#include "Input.h"
#include "Int2.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "System.h"
#include "TextureLoader.h"
#include "Window.h"

using namespace std;

Engine::Engine() : isBeingUnloaded(false)
{
	system = make_unique<System>(*this);
	
	// Create windows
	editorWindow = make_unique<Window>(*system->CreateMainWindow(0, 0, 1024, 768, "Editor"));

	engineWindow = make_unique<Window>(*system->CreateChildWindow(0, 0, 640, 480, "Engine", *editorWindow.get()));

	auto engineRect = engineWindow->GetRect();
	hierachyWindow = make_unique<Window>(*system->CreateChildWindow(engineRect.GetWidth(), 0, 300, engineRect.GetHeight(), "Hierachy", *editorWindow.get()));

	auto hierachyRect = hierachyWindow->GetRect();
	auto editorRect = editorWindow->GetViewport();
	inspectorWindow = make_unique<Window>(*system->CreateChildWindow(hierachyRect.right, 0, editorRect.GetWidth() - hierachyRect.right, editorRect.GetHeight(), "Inspector", *editorWindow.get()));

	auto inspectorRect = inspectorWindow->GetRect();
	projectWindow = make_unique<Window>(*system->CreateChildWindow(0, engineRect.GetHeight(), editorRect.GetWidth() - inspectorRect.GetWidth(), editorRect.GetHeight() - engineRect.GetHeight(), "Project", *editorWindow.get()));

	system->RegisterRawInputDevice(editorWindow->GetHandle());

	auto engineViewport = engineWindow->GetViewport();
	graphics = make_unique<D3D11GraphicsDevice>(engineViewport.GetWidth(), engineViewport.GetHeight(), true, *engineWindow.get(), false);
	input = make_unique<Input>();

	textureLoader = make_unique<TextureLoader>();
	resourceManager = make_unique<ResourceManager>(*textureLoader.get(), *graphics.get());

	auto& scene = *new Scene("Assets/Scene.scene", *this, *resourceManager.get(), *graphics.get(), *input.get());
	AddScene(scene);
}

Engine::~Engine()
{
}

bool Engine::Update()
{
	// Update system messages
	system->Update();

	if (system->GetQuit())
		return false;

	// Update scene
	for (auto behaviour : behaviours)
		behaviour->Update();

	input->Reset();

	// Clear the buffers to begin the scene.
	graphics->ClearRenderTarget(0.0f, 0.5f, 1.0f, 1.0f);

	for (auto camera : cameras)
	{
		camera->Render(graphics->GetGraphicsContext());

		for (auto renderer : renderers)
		{
			renderer->Render(graphics->GetGraphicsContext());
		}
	}

	// Present the rendered scene to the screen.
	graphics->EndScene();

	return true;
}

void Engine::KeyDown(int key)
{
	if (key == VK_ESCAPE)
	{
		system->Quit();
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