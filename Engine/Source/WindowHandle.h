#include <windows.h>

class WindowHandle
{
public:
	WindowHandle(HWND hwnd);
	HWND GetHandle() const;

private:
	const HWND& hwnd;
};