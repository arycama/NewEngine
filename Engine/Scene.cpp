#include "Entity.h"
#include "Scene.h"

#include <assert.h>

using namespace std;

Entity& Scene::CreateEntity()
{
	const auto entity = new Entity();
	entities.push_back(unique_ptr<Entity>(entity));
	return *entity;
}
