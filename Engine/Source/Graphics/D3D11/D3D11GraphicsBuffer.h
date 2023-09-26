#pragma once

#include "GraphicsBuffer.h"
#include <wrl/client.h>

enum D3D11_BIND_FLAG;
enum D3D11_USAGE;
enum D3D11_CPU_ACCESS_FLAG;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class D3D11GraphicsBuffer : public GraphicsBuffer
{
public:
	D3D11GraphicsBuffer(ID3D11Device& device, ID3D11DeviceContext& context, int size, D3D11_BIND_FLAG type, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG access, const D3D11_SUBRESOURCE_DATA* initialData);

private:
	ID3D11DeviceContext& context;
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;

	// Inherited via GraphicsBuffer
	virtual void* BeginWrite() override;
	virtual void EndWrite() override;
};