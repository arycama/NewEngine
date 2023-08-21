#pragma once

#include <memory>
#include <vector>

class Engine
{
public:
	Engine(class System& system);
	~Engine();

	void Update();

	void KeyDown(const unsigned int key);
	void KeyUp(const unsigned int key);

	void AddBehaviour(class Behaviour& behaviour);
	void RemoveBehaviour(class Behaviour& behaviour);

	void AddCamera(class Camera& camera);
	void RemoveCamera(class Camera& camera);

	void AddRenderer(class Renderer& renderer);
	void RemoveRenderer(class Renderer& renderer);

	void AddScene(class Scene& scene);
	void RemoveScene(class Scene& scene);

private:
	System& system;

	std::unique_ptr<class Input> input;
	std::unique_ptr<class Graphics> graphics;
	std::unique_ptr<class Shader> shader;
	std::unique_ptr<class Texture> texture;
	std::unique_ptr<class WindowHandle> windowHandle;

	std::vector<class Behaviour*> behaviours;
	std::vector<class Camera*> cameras;
	std::vector<class Renderer*> renderers;
	std::vector<std::unique_ptr<class Scene>> scenes;
};
