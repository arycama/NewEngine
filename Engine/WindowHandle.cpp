#include "WindowHandle.h"

WindowHandle::WindowHandle(HWND hwnd) : hwnd(hwnd)
{
}

HWND WindowHandle::GetHandle()
{
	return hwnd;
}