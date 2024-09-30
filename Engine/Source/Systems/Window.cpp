#include "Window.h"
#include <windef.h>

using namespace std;

Window::Window(HWND hwnd, const string& name) : hwnd(hwnd), name(name)
{
}

Window::~Window()
{
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)nullptr);
	DestroyWindow(hwnd);
	UnregisterClass(name.c_str(), GetModuleHandle(nullptr));
}

const HWND Window::GetHandle() const
{
	return hwnd;
}

const string& Window::GetName() const
{
	return name;
}

Rect Window::GetRect() const
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hwnd), (LPPOINT)&rect, 2);
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

Rect Window::GetViewport() const
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}