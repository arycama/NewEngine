#pragma once

#include <memory>
#include <vector>

class Scene
{
public:
	class Entity& CreateEntity();

private:
	std::vector<std::unique_ptr<Entity>> entities;
};