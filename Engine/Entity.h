#pragma once

#include <memory>
#include <string>
#include <vector>

class Component;
class Scene;

class Entity
{
public:
	Entity(const std::string& name, Scene& scene);
	~Entity();

	void AddComponent(const Component& component);

private:
	Scene& scene;
	const std::string& name;
	std::vector<std::unique_ptr<const Component>> components;
};