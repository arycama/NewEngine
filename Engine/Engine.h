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

	class Scene& CreateScene();

private:
	System& system;

	std::unique_ptr<class Input> input;
	std::unique_ptr<class Renderer> renderer;
	std::unique_ptr<class Model> model;
	std::unique_ptr<class TextureShader> textureShader;
	std::unique_ptr<class WindowHandle> windowHandle;

	std::vector<std::unique_ptr<class Scene>> scenes;
	std::vector<class Camera*> cameras;
	std::vector<class Behaviour*> behaviours;

	void AddComponent(class Entity* entity, class Behaviour* behaviour);
	void AddComponent(class Entity* entity, class Camera* camera);
	void AddComponent(class Entity* entity, class Component* component);
};
