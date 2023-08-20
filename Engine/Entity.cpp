#include "Component.h"
#include "Entity.h"
#include "Scene.h"

#include <assert.h>
#include <stdexcept>

using namespace std;

void Entity::AddComponent(Component* component)
{
	assert(component);
	components.push_back(unique_ptr<Component>(component));
}
