#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <windows.h>

class Engine;

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class System
{
public:
	System();
	~System();

	void Update();
	void Quit();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	bool quit;

	std::unique_ptr<Engine> engine;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};
