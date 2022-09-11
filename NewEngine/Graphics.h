#pragma once

#include <windows.h>

#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "Input.h"
#include "LightShader.h"
#include "Light.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics(int, int, HWND, Input*);
	~Graphics();

	void Frame();

private:
	D3D* d3d;
	Camera* camera;
	Model* model;
	//TextureShader* textureShader;
	LightShader* lightShader;
	Light* light;
};
