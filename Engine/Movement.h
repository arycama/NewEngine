#pragma once

#include "Behaviour.h"

class Movement : public Behaviour
{
public:
	Movement(class Input& input, class Transform& transform);

	void Update() override;

private:
	class Input& input;
	class Transform& transform;
};
