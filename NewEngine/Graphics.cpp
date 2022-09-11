#include "Graphics.h"

using namespace DirectX;
using namespace PackedVector;

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, Input* input)
{
	d3d = new D3D(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	camera = new Camera(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), input);
	model = new Model(d3d->GetDevice(), L"../NewEngine/Data/seafloor.dds");
	//textureShader = new TextureShader(d3d->GetDevice(), hwnd);

	lightShader = new LightShader(d3d->GetDevice(), hwnd);
	light = new Light(XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f));
}

Graphics::~Graphics()
{
	delete d3d;
	delete camera;
	delete model;
	//delete textureShader;
	delete lightShader;
	delete light;
}

void Graphics::Frame()
{
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Clear the buffers to begin the scene.
	d3d->BeginScene(XMCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(d3d->GetDeviceContext());

	// Get the world, view, and projection matrices from the camera and d3d objects.
	auto viewMatrix = camera->GetViewMatrix();
	auto worldMatrix = d3d->GetWorldMatrix();
	auto projectionMatrix = d3d->GetProjectionMatrix();

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Render the model using the color shader.
	//textureShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());

	// Render the model using the light shader.
	lightShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture(), light->GetDirection(), light->GetColor());

	// Present the rendered scene to the screen.
	d3d->EndScene();
}
