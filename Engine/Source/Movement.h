#pragma once

#include "Behaviour.h"

class Engine;
class Input;
class Transform;

class Movement : public Behaviour
{
public:
	Movement(const Input& input, Transform& transform, Engine& engine);

	void Update() override;

private:
	const Input& input;
	Transform& transform;
};