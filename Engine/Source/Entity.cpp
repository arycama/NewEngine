#include "Camera.h"
#include "Component.h"
#include "Engine.h"
#include "Entity.h"
#include "Input.h"
#include "Movement.h"
#include "Scene.h"
#include "Transform.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <assert.h>
#include <d3d11.h>
#include <fstream>

using namespace std;

Entity::Entity(Scene& scene) : scene(scene), path("")
{
	scene.AddEntity(*this);
}

Entity::Entity(const string& path, Scene& scene, ResourceManager& resourceManager, Engine& engine, ID3D11Device& device, ID3D11DeviceContext& context, const Graphics& graphics, const Input& input) : path(path), scene(scene)
{
	ifstream stream(path);
	while (!stream.eof())
	{
		string componentType;
		stream >> componentType;

		if (componentType == "camera")
			this->AddComponent<Camera>(stream, graphics, engine, device, context, *this);

		if(componentType == "movement")
			this->AddComponent<Movement>(engine, *this, input, stream);
		
		if (componentType == "renderer")
			this->AddComponent<Renderer>(stream, resourceManager, engine, device, context, *this);

		if (componentType == "transform")
			this->AddComponent<Transform>(stream);
	}

	scene.AddEntity(*this);
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

const string& Entity::GetPath() const
{
	return path;
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
