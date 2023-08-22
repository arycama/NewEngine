#pragma once

#include <memory>
#include <vector>

class Engine
{
public:
	Engine(class System& system);
	~Engine();

	void Update();

	void KeyDown(int key);
	void KeyUp(int key);

	void AddBehaviour(class Behaviour& behaviour);
	void RemoveBehaviour(class Behaviour& behaviour);

	void AddCamera(class Camera& camera);
	void RemoveCamera(class Camera& camera);

	void AddRenderer(class Renderer& renderer);
	void RemoveRenderer(class Renderer& renderer);

	void AddScene(class Scene& scene);
	void RemoveScene(class Scene& scene);

private:
	bool isBeingUnloaded;
	System& system;

	std::unique_ptr<class Input> input;
	std::unique_ptr<const class Graphics> graphics;
	std::unique_ptr<const class Shader> shader;
	std::unique_ptr<const class Texture> texture;
	std::unique_ptr<const class WindowHandle> windowHandle;

	std::vector<class Behaviour*> behaviours;
	std::vector<const class Camera*> cameras;
	std::vector<const class Renderer*> renderers;
	std::vector<std::unique_ptr<const class Scene>> scenes;
};
