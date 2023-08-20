#pragma once

#include "Component.h"

class Behaviour : public Component
{
public:
	virtual void Update() = 0;
};
