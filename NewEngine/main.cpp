#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	auto system = new System();
	system->Run();
	delete system;
	return 0;
}