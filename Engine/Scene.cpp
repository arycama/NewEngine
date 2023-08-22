#include "Engine.h"
#include "Entity.h"
#include "Scene.h"
#include <memory>
#include <vector>

using namespace std;

Scene::Scene(Engine& engine) : engine(engine) { };

void Scene::AddEntity(const Entity& entity)
{
	entities.push_back(unique_ptr<const Entity>(&entity));
}

void Scene::RemoveEntity(const Entity& entity)
{
	//entities.erase(find_if(entities.begin(), entities.end(), [entity](std::unique_ptr<Entity> const& i) { return i.get() == &entity; }));
}
