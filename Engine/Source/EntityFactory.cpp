#include "Entity.h"
#include "EntityFactory.h"
#include "Scene.h"

#include <assert.h>
#include <fstream>

using namespace std;

unique_ptr<Entity> EntityFactory::CreateEntity(const string& path, Scene& scene)
{
	ifstream file(path);
	assert(file.is_open());

	auto entity = make_unique<Entity>(path, scene);

	string line;
	while (getline(file, line))
	{

	}

	file.close();

	return entity;
}
