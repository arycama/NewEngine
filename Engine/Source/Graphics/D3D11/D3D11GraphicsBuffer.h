#pragma once

#include "GraphicsBuffer.h"
#include <wrl/client.h>

struct ID3D11Buffer;

class D3D11GraphicsBuffer : public GraphicsBuffer
{
public:
	D3D11GraphicsBuffer(int size, GraphicsBufferCpuAccess access, GraphicsBufferType type, GraphicsBufferUsage usage);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;
};