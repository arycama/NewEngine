#include "Graphics.h"

using namespace std;
using namespace DirectX;

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create and initialize the Direct3D object.
	d3d = unique_ptr<D3D>(new D3D(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR));

	// Create the camera object and set the initial position of the camera.
	camera = unique_ptr<Camera>(new Camera(XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

	// Create and initialize the model object.
	model = unique_ptr<Model>(new Model(d3d->GetDevice(), d3d->GetDeviceContext(), "../Engine/data/stone01.tga"));

	// Create and initialize the texture shader object.
	textureShader = unique_ptr<TextureShader>(new TextureShader(d3d->GetDevice(), hwnd));
}

void Graphics::Update()
{
	// Clear the buffers to begin the scene.
	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	// Get the world, view, and projection matrices from the camera and d3d objects.

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(d3d->GetDeviceContext());

	// Render the model using the texture shader.
	textureShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(), d3d->GetWorldMatrix(), camera->GetViewMatrix(), d3d->GetProjectionMatrix(), model->GetTexture());

	// Present the rendered scene to the screen.
	d3d->EndScene();
}