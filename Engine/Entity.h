#pragma once

#include <memory>
#include <vector>
#include "Component.h"

class Entity
{
public:
	Entity(class Scene& scene);
	~Entity();

	void AddComponent(class Component& component);

private:
	class Scene& scene;
	std::vector<std::unique_ptr<class Component>> components;
};