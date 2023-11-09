#include <string>
#include <windows.h>

#include "Rect.h"

class Window
{
public:
	Window(HWND hwnd, const std::string& name);

	const HWND GetHandle() const;
	const std::string& GetName() const;

	Rect GetRect() const;
	Rect GetLocalRect() const;

private:
	const HWND hwnd;
	const std::string name;
};