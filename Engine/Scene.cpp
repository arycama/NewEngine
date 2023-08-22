#include "Engine.h"
#include "Entity.h"
#include "Scene.h"
#include <memory>
#include <vector>

using namespace std;

Scene::Scene(Engine& engine) : engine(engine), isBeingUnloaded(false) { }

Scene::~Scene()
{
	engine.RemoveScene(*this);
	isBeingUnloaded = true;
}

void Scene::AddEntity(const Entity& entity)
{
	entities.push_back(unique_ptr<const Entity>(&entity));
}

void Scene::RemoveEntity(const Entity& entity)
{
	if (isBeingUnloaded)
		return;

	auto result = find_if(entities.begin(), entities.end(), [&](auto& obj) { return obj.get() == &entity; });
	result->release();
	entities.erase(result);
}
