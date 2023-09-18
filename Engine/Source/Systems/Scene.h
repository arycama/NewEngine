#pragma once

#include <memory>
#include <string>
#include <vector>

class Engine;
class Entity;
class GraphicsDevice;
class Input;
class ResourceManager;

class Scene
{
public:
	Scene(Engine& engine);
	Scene(const std::string& path, Engine& engine, ResourceManager& resourceManager, GraphicsDevice& graphics, Input& input);
	~Scene();

	void Serialize(const std::string& path) const;

	void AddEntity(const Entity& entitiy);
	void RemoveEntity(const Entity& entitiy);

private:
	bool isBeingUnloaded;
	Engine& engine;
	std::vector<std::unique_ptr<const Entity>> entities;
};