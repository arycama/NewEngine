#pragma once

#include <memory>
#include <vector>

class Behaviour;
class Camera;
class GraphicsDevice;
class Input;
class Renderer;
class ResourceManager;
class Scene;
class System;
class TextureLoader;
class Window;

class Engine
{
public:
	Engine();

	bool Update();

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

	std::unique_ptr<System> system;
	std::unique_ptr<Input> input;
	std::unique_ptr<GraphicsDevice> graphics;
	std::unique_ptr<const Window> editorWindow;
	std::unique_ptr<const Window> engineWindow;
	std::unique_ptr<const Window> hierachyWindow;
	std::unique_ptr<const Window> inspectorWindow;
	std::unique_ptr<const Window> projectWindow;
	std::unique_ptr<const TextureLoader> textureLoader;
	std::unique_ptr<ResourceManager> resourceManager;

	std::vector<Behaviour*> behaviours;
	std::vector<const Camera*> cameras;
	std::vector<const Renderer*> renderers;
	std::vector<std::unique_ptr<const Scene>> scenes;
};