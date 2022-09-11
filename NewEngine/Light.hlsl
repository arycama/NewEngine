struct VertexInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

Texture2D shaderTexture;
SamplerState SampleType;

PixelInput Vertex(VertexInput input)
{
	PixelInput output;
	
	input.position.w = 1.0;
	
	output.position = mul(worldMatrix, input.position);
	output.position = mul(viewMatrix, output.position);
	output.position = mul(projectionMatrix, output.position);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	
	output.normal = normalize(output.normal);
	
	return output;
}

float4 Pixel(PixelInput input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.uv);
	
	float3 lightDir = -lightDirection;
	
	float lightIntensity = saturate(dot(input.normal, lightDir));
	
	float4 color = saturate(diffuseColor * lightIntensity);
	
	color *= textureColor;
	
	return color;
}