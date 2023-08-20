#include "Entity.h"
#include "Component.h"

#include <assert.h>
#include <stdexcept>

Entity::Entity(Engine& engine) : engine(engine)
{
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
