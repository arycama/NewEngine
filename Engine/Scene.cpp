#include "Engine.h"
#include "Entity.h"
#include "Scene.h"

#include <assert.h>

using namespace std;

Scene::Scene(Engine& engine) : engine(engine) { };

void Scene::AddEntity(Entity& entity)
{
	entities.push_back(unique_ptr<Entity>(&entity));
}
