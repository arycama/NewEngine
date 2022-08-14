#pragma once

#include <windows.h>

#include "D3D.h"

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics(int, int, HWND);
	~Graphics();

	bool Frame();

private:
	bool Render();

	D3D* d3d;
};
