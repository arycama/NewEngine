#pragma once

#include <memory>

class Camera;
class D3D;
class Input;
class Model;
class System;
class TextureShader;
class WindowHandle;

class Engine
{
public:
	Engine(System& system);
	~Engine();

	void Update() const;

	void KeyDown(const unsigned int key);
	void KeyUp(unsigned int key);

private:
	System& system;

	std::unique_ptr<Input> input;
	std::unique_ptr<D3D> d3d;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Model> model;
	std::unique_ptr<TextureShader> textureShader;
	std::unique_ptr<WindowHandle> windowHandle;
};
