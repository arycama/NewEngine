#pragma once

#include "Behaviour.h"
#include <fstream>
#include <string>

class Engine;
class Entity;
class Input;
class Transform;

class Movement : public Behaviour
{
public:
	Movement(Engine& engine, const Entity& entity, const Input& input, Transform& transform);
	Movement(Engine& engine, const Entity& entity, const Input& input, std::istream& stream);

	void Serialize(std::ostream& stream) const override;
	void Update() override;

private:
	const Entity& entity;
	const Input& input;
	Transform* transform;
};