#pragma once

#include <memory>
#include <string>

class Entity;
class Scene;

class EntityFactory
{
	std::unique_ptr<Entity> CreateEntity(const std::string& path, Scene& scene);
};