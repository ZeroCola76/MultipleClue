cbuffer cbPerObjectVS : register(b0)
{
    matrix WorldViewProjection : packoffset(c0);
    matrix World : packoffset(c4);
}

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VS_INPUT
{
    float3 Position : POSITIONT;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};

/// Vertex Shader
VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    Output.Position = mul(float4(input.Position, 0.f), WorldViewProjection);
    
    Output.Color = input.Color;
    
    Output.Normal = mul(input.Normal, (float3x3) World);
    
    Output.UV = input.UV;
    
    return Output;
}

/// Pixel Shaders

float Color(VS_OUTPUT input) : SV_TARTGET0
{
    return input.Color;
}