#include "WindowHandle.h"
#include <windef.h>

WindowHandle::WindowHandle(HWND hwnd) : hwnd(hwnd)
{
}

HWND WindowHandle::GetHandle() const
{
	return hwnd;
}