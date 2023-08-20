#pragma once

class Input
{
public:
	Input();

	void SetKeyDown(unsigned int);
	void SetKeyUp(unsigned int);
	bool IsKeyDown(unsigned int) const;

private:
	bool keys[256];
};
