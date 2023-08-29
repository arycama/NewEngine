#include "Input.h"
#include "Int2.h"

Input::Input() : mousePosition(Int2(0, 0)), mouseDelta(Int2(0, 0))
{
	// Initialize all the keys to being released and not pressed.
	for (auto i = 0; i < 256; i++)
		keys[i] = false;
}

void Input::SetKeyDown(const unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	keys[input] = true;
}

void Input::SetKeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	keys[input] = false;
}

bool Input::IsKeyDown(const unsigned int key) const
{
	// Return what state the key is in (pressed/not pressed).
	return keys[key];
}

const Int2 Input::GetMousePos() const
{
	return mousePosition;
}

const Int2 Input::GetMouseDelta() const
{
	return mouseDelta;
}

void Input::Reset()
{
	mouseDelta = Int2(0, 0);
}

void Input::SetMousePosition(Int2 mousePosition)
{
	this->mousePosition = mousePosition;
}

void Input::SetMouseDelta(Int2 mouseDelta)
{
	this->mouseDelta = this->mouseDelta + mouseDelta;
}
