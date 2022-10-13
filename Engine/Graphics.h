#pragma once

#include "Camera.h"
#include "D3D.h"
#include "Model.h"
#include "TextureShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics(int, int, HWND);
	void Update() const;

private:
	std::unique_ptr<D3D> d3d;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Model> model;
	std::unique_ptr<TextureShader> textureShader;
};
