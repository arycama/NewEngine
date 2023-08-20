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

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);

	void AddCamera(class Camera* camera);
	void RemoveCamera(class Camera* camera);

	void AddBehaviour(class Behaviour* behaviour);
	void RemoveBehaviour(class Behaviour* behaviour);

private:
	System& system;

	std::unique_ptr<class Input> input;
	std::unique_ptr<class Renderer> renderer;
	std::unique_ptr<class Model> model;
	std::unique_ptr<class TextureShader> textureShader;
	std::unique_ptr<class WindowHandle> windowHandle;

	std::vector<class Entity*> entities;
	std::vector<class Camera*> cameras;
	std::vector<class Behaviour*> behaviours;
};
