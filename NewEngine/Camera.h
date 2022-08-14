#pragma once

#include <d3dx10math.h>

class Camera
{
public:
	Camera(D3DXVECTOR3 position, D3DXVECTOR3 rotation);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXMATRIX GetViewMatrix();

	void Render();

private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXMATRIX viewMatrix;
};
