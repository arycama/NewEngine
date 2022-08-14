#include "Graphics.h"

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd)
{
	d3d = new D3D(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	camera = new Camera();
	camera->SetPosition(0.0f, 0.0f, -10.0f);

	model = new Model(d3d->GetDevice());

	colorShader = new ColorShader(d3d->GetDevice(), hwnd);
}

Graphics::~Graphics()
{
	delete d3d;
	delete camera;
	delete model;
	delete colorShader;
}

void Graphics::Frame()
{
	// Clear the buffers to begin the scene.
	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	camera->GetViewMatrix(viewMatrix);
	d3d->GetWorldMatrix(worldMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(d3d->GetDeviceContext());

	// Render the model using the color shader.
	colorShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

	// Present the rendered scene to the screen.
	d3d->EndScene();
}
