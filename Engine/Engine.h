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
	void AddCamera(class Camera& camera);
	void AddScene(class Scene& scene);

	void RemoveBehaviour(class Behaviour& behaviour);
	void RemoveCamera(class Camera& camera);
	void RemoveScene(class Scene& scene);

private:
	System& system;

	std::unique_ptr<class Input> input;
	std::unique_ptr<class Renderer> renderer;
	std::unique_ptr<class Model> model;
	std::unique_ptr<class TextureShader> textureShader;
	std::unique_ptr<class WindowHandle> windowHandle;

	std::vector<class Behaviour*> behaviours;
	std::vector<class Camera*> cameras;
	std::vector<std::unique_ptr<class Scene>> scenes;
};
