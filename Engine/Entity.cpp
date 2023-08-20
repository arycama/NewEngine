#include "Entity.h"
#include "Component.h"
#include "Engine.h"

#include <assert.h>
#include <stdexcept>

Entity::Entity(Engine& engine) : engine(engine)
{
	engine.AddEntity(this);
}

Entity::~Entity()
{
	for (auto component : components)
	{
		delete component;
	}
}

void Entity::AddComponent(Component* component)
{
	assert(component);
	components.push_back(component);
}
