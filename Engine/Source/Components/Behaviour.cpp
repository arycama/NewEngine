#include "Behaviour.h"
#include "../Engine.h"

Behaviour::Behaviour(Engine& engine) : engine(engine)
{
	engine.AddBehaviour(*this);
}

Behaviour::~Behaviour()
{
	engine.RemoveBehaviour(*this);
}