#include "Input.h"

Input::Input()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}

void Input::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	keys[input] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return keys[key];
}
