#include "Component.h"
#include "Entity.h"
#include "Scene.h"

#include <assert.h>
#include <fstream>

using namespace std;

Entity::Entity(const string& name, Scene& scene) : name(name), scene(scene)
{
	scene.AddEntity(*this);
}

Entity::Entity(const string& path, const string& name, Scene& scene) : Entity(name, scene)
{

}

Entity::~Entity()
{
	scene.RemoveEntity(*this);
}

void Entity::Serialize(const string& path) const
{
	ofstream file(path);
	assert(file.is_open());

	for (auto& component : components)
	{
		component->Serialize(file);
		file << endl;
	}
}

int Entity::GetComponentIndex(const Component& component) const
{
	auto index = find_if(components.begin(), components.end(), [&](auto& obj) { return obj.get() == &component; });
	return distance(components.begin(), index);
}

Component& Entity::GetComponentAt(int index) const
{
	return *components.at(index).get();
}
