#pragma once

#include "Behaviour.h"

class Movement : public Behaviour
{
public:
	Movement(const class Input& input, class Transform& transform, class Engine& engine);

	void Update() override;

private:
	const class Input& input;
	class Transform& transform;
};
