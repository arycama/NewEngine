#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <string>
#include <windows.h>

class Engine;
class Window;

class System
{
public:
	System(Engine& engine);

	bool GetQuit() const;

	int GetScreenWidth() const;
	int GetScreenHeight() const;

	Window* CreateMainWindow(int x, int y, int width, int height, const std::string& name);
	Window* CreateChildWindow(int x, int y, int width, int height, const std::string& name, const Window& parent);
	void RegisterRawInputDevice(HWND hwnd);
	void ReleaseWindow(const Window& handle);

	void Update();

	void ToggleCursor(bool isVisible);

	void Quit();

private:
	bool quit;

	Engine& engine;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(HWND, UINT, WPARAM, LPARAM);
};
