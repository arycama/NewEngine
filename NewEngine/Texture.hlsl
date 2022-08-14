cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(worldMatrix, input.position);
	output.position = mul(viewMatrix, output.position);
	output.position = mul(projectionMatrix, output.position);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    return output;
}

Texture2D shaderTexture;
SamplerState SampleType;

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}
