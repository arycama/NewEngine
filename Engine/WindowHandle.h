#include <windows.h>

class WindowHandle
{
public:
	WindowHandle(HWND hwnd);
	HWND GetHandle();

private:
	HWND& hwnd;
};