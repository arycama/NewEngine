#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <memory>

#include "Input.h"
#include "Graphics.h"

class System
{
public:
	System();
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	std::unique_ptr<Input> input;
	std::unique_ptr<Graphics> graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;
