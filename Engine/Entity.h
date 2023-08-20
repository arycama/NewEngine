#pragma once

#include <vector>

class Entity
{
public:
	Entity(class Engine& engine);
	~Entity();

	void AddComponent(class Component* component);

private:
	class Engine& engine;
	std::vector<class Component*> components;
};