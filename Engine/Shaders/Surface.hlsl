struct VertexInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

cbuffer PerCameraData
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer PerDrawData
{
	matrix worldMatrix;
};

Texture2D shaderTexture;
SamplerState SampleType;

PixelInput Vertex(VertexInput input)
{
	float3 worldPosition = mul(worldMatrix, float4(input.position, 1.0)).xyz;
	float3 viewPosition = mul(viewMatrix, float4(worldPosition, 1.0)).xyz;
	
    PixelInput output;
	output.position = mul(projectionMatrix, float4(viewPosition, 1.0));
    output.uv = input.uv;
	output.normal = input.normal;
    return output;
}

float3 Pixel(PixelInput input) : SV_TARGET
{
	float3 color = shaderTexture.Sample(SampleType, input.uv);
	float3 L = normalize(float3(0.75, 0.75, -0.75));
	float3 N = normalize(input.normal);
	float3 C = 1.0;
	float3 A = 0.25;
	color *= saturate(dot(N, L)) * C + A;
	return color;
}
