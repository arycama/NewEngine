#pragma once

#include <memory>
#include <vector>

class Engine;
class Entity;

class Scene
{
public:
	Scene(Engine& engine);
	~Scene();

	void AddEntity(const Entity& entitiy);
	void RemoveEntity(const Entity& entitiy);

private:
	bool isBeingUnloaded;
	Engine& engine;
	std::vector<std::unique_ptr<const Entity>> entities;
};