#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Component.h"

class Entity
{
public:
	Entity(const std::string& name, class Scene& scene);
	~Entity();

	void AddComponent(class Component& component);

private:
	class Scene& scene;
	const std::string& name;
	std::vector<std::unique_ptr<const class Component>> components;
};