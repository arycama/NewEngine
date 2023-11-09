#include "Window.h"
#include <windef.h>

using namespace std;

Window::Window(HWND hwnd, const string& name) : hwnd(hwnd), name(name)
{
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
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}

Rect Window::GetLocalRect() const
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return Rect(rect.left, rect.top, rect.right, rect.bottom);
}