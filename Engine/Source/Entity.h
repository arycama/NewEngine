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

	template <class T, class... K> T& AddComponent(K&&... _Args)
	{
		auto component = new T(std::forward<K>(_Args)...);
		components.push_back(std::unique_ptr<const Component>(component));
		return *component;
	}

private:
	Scene& scene;
	const std::string& name;
	std::vector<std::unique_ptr<const Component>> components;
};