#pragma once

enum class GraphicsBufferCpuAccess
{
	None,
	Write = 0x100000,
	Read = 0x200000
};