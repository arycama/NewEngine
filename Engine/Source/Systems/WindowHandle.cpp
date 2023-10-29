#include "WindowHandle.h"
#include <windef.h>

using namespace std;

WindowHandle::WindowHandle(HWND hwnd, const string& name) : hwnd(hwnd), name(name)
{
}

const HWND WindowHandle::GetHandle() const
{
	return hwnd;
}

const string& WindowHandle::GetName() const
{
	return name;
}