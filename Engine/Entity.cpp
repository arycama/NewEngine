#include "Behaviour.h"
#include "Camera.h"
#include "Component.h"
#include "Engine.h"
#include "Entity.h"
#include "Scene.h"

using namespace std;

Entity::Entity(Scene& scene, Engine& engine) : engine(engine)
{
	scene.AddEntity(*this);
}

void Entity::AddComponent(Behaviour& behaviour)
{
	components.push_back(unique_ptr<Component>(&behaviour));
}

void Entity::AddComponent(Camera& camera)
{
	components.push_back(unique_ptr<Component>(&camera));
}

void Entity::AddComponent(Component& component)
{
	components.push_back(unique_ptr<Component>(&component));
}
