#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <windows.h>

#include "Graphics.h"
#include "Input.h"

class System
{
public:
	System();
	~System();

	void Run() const;

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM) const;

private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	std::unique_ptr<Input> input;
	std::unique_ptr<Graphics> graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = nullptr;
