#pragma once

#include "Component.h"

class Behaviour : public Component
{
public:
	Behaviour(class Engine& engine);

	virtual void Update() = 0;

private:
	Engine& engine;
};
