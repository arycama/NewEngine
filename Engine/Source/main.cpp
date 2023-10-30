//#define _CRTDBG_MAP_ALLOC

#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <crtdbg.h>
#include <minwindef.h>
#include <winnt.h>
#include <memory>

#include "Engine.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto engine = make_unique<Engine>();

	while (true)
	{
		if (!engine->Update())
			break;
	}

	return 0;
}