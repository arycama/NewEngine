#include "Graphics.h"

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd)
{
	d3d = new D3D(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	camera = new Camera(D3DXVECTOR3(0.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	model = new Model(d3d->GetDevice(), L"../NewEngine/Data/seafloor.dds");
	textureShader = new TextureShader(d3d->GetDevice(), hwnd);
}

Graphics::~Graphics()
{
	delete d3d;
	delete camera;
	delete model;
	delete textureShader;
}

void Graphics::Frame()
{
	// Clear the buffers to begin the scene.
	d3d->BeginScene(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(d3d->GetDeviceContext());

	// Get the world, view, and projection matrices from the camera and d3d objects.
	auto viewMatrix = camera->GetViewMatrix();
	auto worldMatrix = d3d->GetWorldMatrix();
	auto projectionMatrix = d3d->GetProjectionMatrix();

	// Render the model using the color shader.
	textureShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());

	// Present the rendered scene to the screen.
	d3d->EndScene();
}
