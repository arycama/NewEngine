#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <string>
#include <windows.h>

class WindowHandle;

class System
{
public:
	System();
	~System();

	int GetScreenWidth() const;
	int GetScreenHeight() const;

	void ToggleFullscreen(bool isFullscreen);

	HWND InitializeWindow(int x, int y, int width, int height, const std::string& name);
	void RegisterRawInputDevice(HWND hwnd);
	void ReleaseWindow(const WindowHandle& handle, bool fullScreen);

	void Update();
	void Quit();

private:
	const HINSTANCE hInstance;
	bool quit;

	std::unique_ptr<class Engine> engine;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(HWND, UINT, WPARAM, LPARAM);
};
