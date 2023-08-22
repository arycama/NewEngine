#pragma once

#include "Component.h"

class Engine;

class Behaviour : public Component
{
public:
	Behaviour(Engine& engine);
	~Behaviour();

	virtual void Update() = 0;
	
private:
	Engine& engine;
};
