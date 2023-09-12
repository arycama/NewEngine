#pragma once

#include "Behaviour.h"
#include <fstream>
#include <string>

class Engine;
class Input;
class Transform;

class Movement : public Behaviour
{
public:
	Movement(const Input& input, Transform& transform, Engine& engine);

	void Serialize(std::ofstream& stream) const override;
	void Update() override;

private:
	const Input& input;
	Transform& transform;
};