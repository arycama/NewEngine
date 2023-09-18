#include "Engine.h"
#include "Entity.h"
#include "GraphicsDevice.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <fstream>
#include <memory>
#include <vector>

using namespace std;

Scene::Scene(Engine& engine) : engine(engine), isBeingUnloaded(false) { }

Scene::Scene(const string& path, Engine& engine, ResourceManager& resourceManager, GraphicsDevice& graphicsDevice, Input& input) : Scene(engine)
{
	ifstream stream(path);

	while (!stream.eof())
	{
		string entityPath;
		stream >> entityPath;
		new Entity(entityPath, *this, resourceManager, engine, graphicsDevice, input);
	}
}

Scene::~Scene()
{
	engine.RemoveScene(*this);
	isBeingUnloaded = true;
}

void Scene::Serialize(const string& path) const
{
	ofstream stream(path);

	for (const auto& entity : entities)
	{
		stream << entity->GetPath();

		if (entity != entities.back())
			stream << endl;
	}
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
