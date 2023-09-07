#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

#include "System.h"
#include <minwindef.h>
#include <winnt.h>
#include <memory>

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto system = make_unique<System>();
	system->Update();

	return 0;
}