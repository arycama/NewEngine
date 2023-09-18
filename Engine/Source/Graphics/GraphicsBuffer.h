#pragma once

enum class GraphicsBufferCpuAccess;
enum class GraphicsBufferType;
enum class GraphicsBufferUsage;

class GraphicsBuffer
{
public:
	virtual void* BeginWrite() = 0;
	virtual void EndWrite() = 0;
};
