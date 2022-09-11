#pragma once

#include <Windows.h>

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
	bool Frame();
	void InitializeWindows(int&, int&);

	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	Input* input;
	Graphics* graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* applicationHandle = 0;
