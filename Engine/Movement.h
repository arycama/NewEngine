#pragma once

#include "Behaviour.h"

class Input;
class Transform;

class Movement : public Behaviour
{
public:
	Movement(Engine& engine, Input& input, Transform& transform);

	void Update() override;

private:
	Input& input;
	Transform& transform;
};
