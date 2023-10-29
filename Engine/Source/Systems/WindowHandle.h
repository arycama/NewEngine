#include <string>
#include <windows.h>

class WindowHandle
{
public:
	WindowHandle(HWND hwnd, const std::string& name);

	const HWND GetHandle() const;
	const std::string& GetName() const;

private:
	const HWND hwnd;
	const std::string name;
};