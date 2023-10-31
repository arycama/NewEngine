#define WIN32_LEAN_AND_MEAN

#include "Engine.h"
#include "System.h"
#include "WindowHandle.h"

#include <hidusage.h>
#include <memory>
#include <string>
#include <windowsx.h>

using namespace std;

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lParam)
{
	const auto system = reinterpret_cast<System*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return system->MessageHandler(hwnd, umessage, wparam, lParam);
}

System::System(Engine& engine) : hInstance(GetModuleHandleA(nullptr)), engine(engine), quit(false)
{	
}

System::~System()
{
	// Remove the application instance.
}

bool System::GetQuit() const
{
	return quit;
}

int System::GetScreenWidth() const
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int System::GetScreenHeight() const
{
	return GetSystemMetrics(SM_CYSCREEN);
}

void System::ToggleFullscreen(bool isFullscreen)
{
	//	// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);

		auto width = GetScreenWidth();
		auto height = GetScreenHeight();

		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(width);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(height);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		//posX = posY = 0;
}

HWND System::InitializeWindow(int x, int y, int width, int height, const string& name, HWND parent)
{
	// Setup the windows class with default settings.
	WNDCLASSEXA wc =
	{
		sizeof(WNDCLASSEXA),
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		System::WndProc,
		0,
		0,
		hInstance,
		LoadIcon(nullptr, IDI_WINLOGO),
		LoadCursor(nullptr, IDC_ARROW),
		static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
		nullptr,
		name.c_str(),
		wc.hIcon
	};

	RegisterClassExA(&wc);

	// Create the window with the screen settings and get the handle to it.
	auto flags = 0ul;// WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	if (parent == nullptr)
		flags |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	else
		flags |= WS_CHILDWINDOW;

	auto hwnd = CreateWindowExA(WS_EX_APPWINDOW, name.c_str(), name.c_str(), flags, x, y, width, height, parent, nullptr, hInstance, nullptr);

	// Set a pointer to this object so that messages can be forwarded
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	
	return hwnd;
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

void System::ReleaseWindow(const WindowHandle& handle, bool fullScreen)
{
	// Fix the display settings if leaving full screen mode.
	if (fullScreen)
		ChangeDisplaySettings(nullptr, 0);

	// Release the pointer to this class.
	SetWindowLongPtr(handle.GetHandle(), GWLP_USERDATA, (LONG_PTR)nullptr);

	// Remove the window.
	DestroyWindow(handle.GetHandle());

	UnregisterClassA(handle.GetName().c_str(), hInstance);
}

void System::Update()
{
	// Handle the windows messages.
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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

		case WM_QUIT:
		{
			Quit();
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}
