#pragma once

#include "Component.h"

class Behaviour : public Component
{
public:
	Behaviour(class Engine& engine);
	~Behaviour();

	virtual void Update() = 0;

private:
	Engine& engine;
};
