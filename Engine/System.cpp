#define WIN32_LEAN_AND_MEAN

#include "System.h"
#include "Engine.h"
#include "WindowHandle.h"

using namespace std;

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	const auto system = reinterpret_cast<System*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return system->MessageHandler(hwnd, umessage, wparam, lparam);
}

System::System()
{	
	// Initialize the windows api.
	// Get the instance of this application.
	hinstance = GetModuleHandle(nullptr);

	// Give the application a name.
	applicationName = L"Engine";

	// Setup the windows class with default settings.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = System::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Create and initialize the graphics object.  This object will handle rendering all the graphics for this application.
	engine = make_unique<Engine>(*this);
}

System::~System()
{
	// Remove the application instance.
	UnregisterClass(applicationName, hinstance);
	hinstance = nullptr;
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
	auto hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, width, height, nullptr, nullptr, hinstance, nullptr);

	// Set a pointer to this object so that messages can be forwarded
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return hwnd;
}

void System::ReleaseWindow(HWND hwnd, bool fullScreen)
{
	// Fix the display settings if leaving full screen mode.
	if (fullScreen)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

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

LRESULT System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
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
			engine->KeyDown(static_cast<int>(wparam));
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			engine->KeyUp(static_cast<int>(wparam));
			return 0;
		}

		case WM_QUIT:
		{
			Quit();
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}
