#pragma once

#include <memory>
#include <vector>

class Behaviour;
class Camera;
class Graphics;
class Input;
class Renderer;
class Scene;
class Shader;
class System;
class Texture;
class WindowHandle;

class Engine
{
public:
	Engine(System& system);
	~Engine();

	void Update();

	void KeyDown(int key);
	void KeyUp(int key);

	void AddBehaviour(Behaviour& behaviour);
	void RemoveBehaviour(Behaviour& behaviour);

	void AddCamera(Camera& camera);
	void RemoveCamera(Camera& camera);

	void AddRenderer(Renderer& renderer);
	void RemoveRenderer(Renderer& renderer);

	void AddScene(Scene& scene);
	void RemoveScene(Scene& scene);

private:
	bool isBeingUnloaded;
	System& system;

	std::unique_ptr<Input> input;
	std::unique_ptr<const Graphics> graphics;
	std::unique_ptr<const Shader> shader;
	std::unique_ptr<const Texture> texture;
	std::unique_ptr<const WindowHandle> windowHandle;

	std::vector<Behaviour*> behaviours;
	std::vector<const Camera*> cameras;
	std::vector<const Renderer*> renderers;
	std::vector<std::unique_ptr<const Scene>> scenes;
};
