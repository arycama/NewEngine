#include "Graphics.h"

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd)
{
	d3d = new D3D(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
}

Graphics::~Graphics()
{
	delete d3d;
}

bool Graphics::Frame()
{
	bool result = Render();
	return result;
}

bool Graphics::Render()
{
	// Clear the buffers to begin the scene.
	d3d->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);

	// Present the rendered scene to the screen.
	d3d->EndScene();

	return true;
}
