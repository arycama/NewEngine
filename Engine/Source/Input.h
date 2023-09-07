#pragma once

#include "Int2.h"

class Input
{
public:
	Input();

	void SetKeyDown(unsigned int);
	void SetKeyUp(unsigned int);
	bool IsKeyDown(unsigned int) const;
	void Reset();

	void SetMousePosition(Int2 mousePosition);
	void SetMouseDelta(Int2 mouseDelta);

	const Int2 GetMousePos() const;
	const Int2 GetMouseDelta() const;

private:
	Int2 mousePosition, mouseDelta;
	bool keys[256];
};
