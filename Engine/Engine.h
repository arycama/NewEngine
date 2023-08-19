#pragma once

#include "Camera.h"
#include "D3D.h"
#include "Input.h"
#include "Model.h"
#include "TextureShader.h"

class System;

class Engine
{
public:
	Engine(System& system);
	~Engine();

	void Update() const;

	void KeyDown(const unsigned int key);
	void KeyUp(unsigned int key);

private:
	HWND hwnd;
	System& system;

	std::unique_ptr<Input> input;
	std::unique_ptr<D3D> d3d;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Model> model;
	std::unique_ptr<TextureShader> textureShader;
};
