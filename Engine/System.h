#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <string>
#include <windows.h>

class System
{
public:
	System();
	~System();

	HWND InitializeWindow(bool fullScreen, int& width, int& height);
	void ReleaseWindow(HWND hwnd, bool fullScreen);

	void Update();
	void Quit();

private:
	const std::string applicationName;
	const HINSTANCE hInstance;
	bool quit;

	std::unique_ptr<class Engine> engine;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	LRESULT  MessageHandler(HWND, UINT, WPARAM, LPARAM);
};
