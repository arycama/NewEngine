#define WIN32_LEAN_AND_MEAN

#include "Engine.h"
#include "System.h"
#include "Window.h"

#include <hidusage.h>
#include <memory>
#include <string>
#include <windowsx.h>

using namespace std;

#define IDM_NEW 1001
#define IDM_OPEN 1002
#define IDM_SAVE 1003
#define IDM_QUIT 1004

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lParam)
{
	const auto system = reinterpret_cast<System*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return system->MessageHandler(hwnd, umessage, wparam, lParam);
}

System::System(Engine& engine) : engine(engine), quit(false)
{	
}

bool System::GetQuit() const
{
	return quit;
}

void System::ToggleCursor(bool isVisible)
{
	ShowCursor(isVisible);
}

int System::GetScreenWidth() const
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int System::GetScreenHeight() const
{
	return GetSystemMetrics(SM_CYSCREEN);
}

Window* System::CreateMainWindow(int x, int y, int width, int height, const string& name)
{
	// Setup the windows class with default settings.
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(WNDCLASSEXA),
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = System::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = name.c_str();
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	RegisterClassEx(&wc);

	auto hMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING, IDM_NEW, "&New");
	AppendMenu(hMenu, MF_STRING, IDM_OPEN, "&Open");
	AppendMenu(hMenu, MF_STRING, IDM_SAVE, "&Save");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, IDM_QUIT, "&Quit");

	auto hMenubar = CreateMenu();
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, "&File");

	// Create the window with the screen settings and get the handle to it.
	auto flags = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	auto hwnd = CreateWindowEx(WS_EX_APPWINDOW, name.c_str(), name.c_str(), flags, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, hMenubar, GetModuleHandle(nullptr), nullptr);

	// Set a pointer to this object so that messages can be forwarded
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	return new Window(hwnd, name);
}

Window* System::CreateChildWindow(int x, int y, int width, int height, const string& name, const Window& parent)
{
	// Setup the windows class with default settings.
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(WNDCLASSEXA),
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = System::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = name.c_str();
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

	RegisterClassEx(&wc);

	// Create the window with the screen settings and get the handle to it.
	auto flags = WS_VISIBLE | WS_CHILD | WS_THICKFRAME;
	auto hwnd = CreateWindowEx(WS_EX_APPWINDOW, name.c_str(), name.c_str(), flags, x, y, width, height, parent.GetHandle(), nullptr, GetModuleHandle(nullptr), nullptr);

	// Set a pointer to this object so that messages can be forwarded
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	// Bring the window up on the screen and set it as main focus.
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	
	return new Window(hwnd, name);
}

void System::RegisterRawInputDevice(HWND hwnd)
{
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hwnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

void System::ReleaseWindow(const Window& handle)
{
	// Release the pointer to this class.
	SetWindowLongPtr(handle.GetHandle(), GWLP_USERDATA, (LONG_PTR)nullptr);

	// Remove the window.
	DestroyWindow(handle.GetHandle());

	UnregisterClass(handle.GetName().c_str(), GetModuleHandle(nullptr));
}

void System::Update()
{
	// Handle the windows messages.
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
		{
			Quit();
			break;
		}
	}
}

void System::Quit()
{
	quit = true;
}

LRESULT System::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// Check if the window is being destroyed or closed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_COMMAND:
		{
			switch (wParam)
			{
				case IDM_NEW:
					break;
				case IDM_OPEN:
					break;
				case IDM_SAVE:
					break;
				case IDM_QUIT:
					PostQuitMessage(0);
					return 0;
			}
		}

		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			engine.KeyDown(static_cast<int>(wParam));
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			engine.KeyUp(static_cast<int>(wParam));
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			engine.SetMousePosition(xPos, yPos);
			return 0;
		}

		case WM_INPUT:
		{
			UINT dwSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
				engine.SetMouseDelta(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

			break;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}
