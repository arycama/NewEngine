#pragma once

#include <memory>
#include <vector>

class Behaviour;
class Camera;
class Graphics;
class Input;
class Renderer;
class ResourceManager;
class Scene;
class System;
class TextureLoader;
class WindowHandle;

class Engine
{
public:
	Engine(System& system);
	~Engine();

	void Update();

	void KeyDown(int key);
	void KeyUp(int key);
	void SetMousePosition(int xPos, int yPos);
	void SetMouseDelta(int xDelta, int yDelta);

	void AddBehaviour(Behaviour& behaviour);
	void RemoveBehaviour(const Behaviour& behaviour);

	void AddCamera(const Camera& camera);
	void RemoveCamera(const Camera& camera);

	void AddRenderer(const Renderer& renderer);
	void RemoveRenderer(const Renderer& renderer);

	void AddScene(const Scene& scene);
	void RemoveScene(const Scene& scene);

private:
	bool isBeingUnloaded;
	System& system;

	std::unique_ptr<Input> input;
	std::unique_ptr<const Graphics> graphics;
	std::unique_ptr<const WindowHandle> windowHandle;
	std::unique_ptr<const TextureLoader> textureLoader;
	std::unique_ptr<ResourceManager> resourceManager;

	std::vector<Behaviour*> behaviours;
	std::vector<const Camera*> cameras;
	std::vector<const Renderer*> renderers;
	std::vector<std::unique_ptr<const Scene>> scenes;
};