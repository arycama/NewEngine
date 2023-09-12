#pragma once

#include <fstream>
#include <string>

class Component
{
public:
	virtual ~Component();

	virtual void Serialize(std::ofstream& stream) const = 0;
};