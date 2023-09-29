#pragma once

class Handle
{
public:
	Handle();
	Handle(int index, int version);

	int GetIndex() const;
	int GetVersion() const;

private:
	int index, version;
};