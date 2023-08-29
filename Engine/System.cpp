#define WIN32_LEAN_AND_MEAN

#include "System.h"
#include "Engine.h"
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

System::System() : hInstance(GetModuleHandleA(nullptr)), applicationName("Engine")
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
		applicationName.c_str(),
		wc.hIcon
	};

	RegisterClassExA(&wc);

	engine = make_unique<Engine>(*this);
}

System::~System()
{
	// Remove the application instance.
	UnregisterClassA(applicationName.c_str(), hInstance);
}

HWND System::InitializeWindow(bool fullScreen, int& width, int& height)
{
	// Determine the resolution of the clients desktop screen.
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	int posX, posY;

	if (fullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(width);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(height);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		width = 800;
		height = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	auto hwnd = CreateWindowExA(WS_EX_APPWINDOW, applicationName.c_str(), applicationName.c_str(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, width, height, nullptr, nullptr, hInstance, nullptr);

	// Set a pointer to this object so that messages can be forwarded
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hwnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	return hwnd;
}

void System::ReleaseWindow(HWND hwnd, bool fullScreen)
{
	// Fix the display settings if leaving full screen mode.
	if (fullScreen)
		ChangeDisplaySettings(nullptr, 0);

	// Release the pointer to this class.
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)nullptr);

	// Remove the window.
	DestroyWindow(hwnd);
}

void System::Update()
{
	// Loop until there is a quit message from the window or the user.
	while(true)
	{
		// Handle the windows messages.
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(quit)
			return;

		engine->Update();

	} while (!quit);
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
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			engine->KeyDown(static_cast<int>(wParam));
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			engine->KeyUp(static_cast<int>(wParam));
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			engine->SetMousePosition(xPos, yPos);
			return 0;
		}

		case WM_INPUT:
		{
			UINT dwSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
				engine->SetMouseDelta(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

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
