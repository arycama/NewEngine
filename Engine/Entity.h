#pragma once

#include <memory>
#include <vector>
#include "Component.h"

class Entity
{
public:
	void AddComponent(class Component* component);

private:
	std::vector<std::unique_ptr<class Component>> components;
};