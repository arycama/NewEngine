#include <d3d11.h>
#include <directxmath.h>

class Light
{
public:
	Light(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 color);

	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT3 GetColor();

private:
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 color;
};