#pragma once

#include "AssetCache.h"
#include <memory>
#include <vector>

class Behaviour;
class Camera;
class Graphics;
class Input;
class RenderController;
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
	std::unique_ptr<const class Material> material;
	std::unique_ptr<const Shader> shader;
	std::unique_ptr<const Texture> texture;
	std::unique_ptr<const WindowHandle> windowHandle;

	//AssetCache<Texture> textures;

	std::vector<Behaviour*> behaviours;
	std::vector<const Camera*> cameras;
	std::vector<const Renderer*> renderers;
	std::vector<std::unique_ptr<const Scene>> scenes;
};
