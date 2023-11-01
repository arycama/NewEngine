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
#include "WindowHandle.h"

using namespace std;

const bool fullScreen = false;

Engine::Engine() : isBeingUnloaded(false)
{
	system = make_unique<System>(*this);
	
	auto editorWidth = 1024;// system->GetScreenWidth();
	auto editorHeight = 768;// system->GetScreenHeight();

	editorWindow = make_unique<WindowHandle>(system->CreateMainWindow(0, 0, editorWidth, editorHeight, "Editor"));

	// TODO: This should be in engine.. editor doesn't need raw input
	system->RegisterRawInputDevice(editorWindow->GetHandle());

	int windowWidth = 800, windowHeight = 600;
	auto posX = 0;// (system->GetScreenWidth() - windowWidth) / 2;
	auto posY = 0;// (system->GetScreenHeight() - windowHeight) / 2;
	engineWindow = make_unique<WindowHandle>(system->CreateChildWindow(posX, posY, windowWidth, windowHeight, "Engine", *editorWindow.get()));

	//system->ToggleFullscreen(true);

	graphics = make_unique<D3D11GraphicsDevice>(windowWidth, windowHeight, true, *engineWindow.get(), fullScreen);
	input = make_unique<Input>();

	textureLoader = make_unique<TextureLoader>();
	resourceManager = make_unique<ResourceManager>(*textureLoader.get(), *graphics.get());

	auto& scene = *new Scene("Assets/Scene.scene", *this, *resourceManager.get(), *graphics.get(), *input.get());
	AddScene(scene);

	//system->ToggleCursor(false);
}

Engine::~Engine()
{
	system->ReleaseWindow(*engineWindow.get(), fullScreen);
	system->ReleaseWindow(*editorWindow.get(), fullScreen);
	//system->ToggleCursor(true);
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