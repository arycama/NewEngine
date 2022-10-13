#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "System.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
#if 0
	auto system = unique_ptr<System>(new System());
#else
	// Not using unique ptr so we can check memory leaks
	auto system = new System();
	system->Run();
	delete system;
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}