#pragma once

#include <memory>
#include <vector>
#include "Component.h"

class Entity
{
public:
	Entity(class Scene& scene, class Engine& engine);
	void AddComponent(class Behaviour& behaviour);
	void AddComponent(class Camera& camera);
	void AddComponent(class Component& component);

private:
	class Engine& engine;
	std::vector<std::unique_ptr<class Component>> components;
};