#include "D3D11GraphicsBuffer.h"

#include <d3d11.h>
#include <comdef.h>

using namespace _com_util;

D3D11GraphicsBuffer::D3D11GraphicsBuffer(int size, GraphicsBufferCpuAccess access, GraphicsBufferType type, GraphicsBufferUsage usage)
{
	//CD3D11_BUFFER_DESC desc(size, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	//CheckError(device.CreateBuffer(&desc, nullptr, &data));
}
