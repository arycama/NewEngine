#pragma once

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool keys[256];
};
