#pragma once

#include <memory>
#include <string>
#include <vector>

class Component;
class Engine;
class GraphicsDevice;
class Input;
class ResourceManager;
class Scene;

class Entity
{
public:
	Entity(Scene& scene);
	Entity(const std::string& path, Scene& scene, ResourceManager& resourceManager, Engine& engine, GraphicsDevice& graphicsDevice, const Input& input);

	~Entity();

	void Serialize(const std::string& path) const;

	int GetComponentIndex(const Component& component) const;
	Component& GetComponentAt(int index) const;
	const std::string& GetPath() const;

	template <class T, class... K> T& AddComponent(K&&... _Args)
	{
		auto component = new T(std::forward<K>(_Args)...);
		components.push_back(std::unique_ptr<Component>(component));
		return *component;
	}

private:
	Scene& scene;
	const std::string path;
	std::vector<std::unique_ptr<Component>> components;
};