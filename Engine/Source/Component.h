#pragma once

#include <fstream>
#include <string>

class Component
{
public:
	virtual ~Component();

	virtual void Serialize(std::ostream& stream) const = 0;
};