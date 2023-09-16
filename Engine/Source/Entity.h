#pragma once

#include <memory>
#include <string>
#include <vector>

class Component;
class Engine;
class ResourceManager;
class Scene;

struct ID3D11Device;
struct ID3D11DeviceContext;

class Entity
{
public:
	Entity(const std::string& name, Scene& scene);
	Entity(const std::string& path, const std::string& name, Scene& scene, ResourceManager& resourceManager, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& context);

	~Entity();

	void Serialize(const std::string& path) const;

	template <class T, class... K> T& AddComponent(K&&... _Args)
	{
		auto component = new T(std::forward<K>(_Args)...);
		components.push_back(std::unique_ptr<Component>(component));
		return *component;
	}

	int GetComponentIndex(const Component& component) const;
	Component& GetComponentAt(int index) const;

private:
	Scene& scene;
	const std::string& name;
	std::vector<std::unique_ptr<Component>> components;
};