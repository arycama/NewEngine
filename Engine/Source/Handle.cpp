#include "Handle.h"

Handle::Handle() : index(-1), version(-1) { }

Handle::Handle(int index, int version) : index(index), version(version) { }

int Handle::GetIndex() const
{
	return index;
}

int Handle::GetVersion() const
{
	return version;
}