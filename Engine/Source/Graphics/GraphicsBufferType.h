#pragma once

enum class GraphicsBufferType
{
	VertexBuffer = 0x1,
	IndexBuffer = 0x2,
	ConstantBuffer = 0x4,
	ShaderResource = 0x8,
	StreamOutput = 0x10,
	RenderTarget = 0x20,
	DepthStencil = 0x40,
	UnorderedAccess = 0x80,
	Decoder = 0x200,
	VideoEncoder = 0x400
};