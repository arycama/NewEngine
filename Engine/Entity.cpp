#include "Behaviour.h"
#include "Camera.h"
#include "Component.h"
#include "Engine.h"
#include "Entity.h"
#include "Scene.h"

using namespace std;

Entity::Entity(Scene& scene) : scene(scene)
{
	scene.AddEntity(*this);
}

Entity::~Entity()
{
	scene.RemoveEntity(*this);
}

void Entity::AddComponent(Component& component)
{
	components.push_back(unique_ptr<Component>(&component));
}
