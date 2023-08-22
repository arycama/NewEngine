#pragma once

#include <memory>
#include <vector>

class Scene
{
public:
	Scene(class Engine& engine);
	void AddEntity(class Entity& entitiy);
	void RemoveEntity(class Entity& entitiy);

private:
	class Engine& engine;
	std::vector<std::unique_ptr<class Entity>> entities;
};