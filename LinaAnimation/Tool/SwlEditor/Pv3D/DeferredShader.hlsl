#include "Header.hlsli"

cbuffer cbPerObjectVS : register(b0)
{
    float4x4 WorldViewProjection : packoffset(c0);
    float4x4 World : packoffset(c4);
}

cbuffer cbPerObjectPS : register(b0)
{
    float specExp : packoffset(c0);
    float specIntensity : packoffset(c0.y);
}

Texture2D DiffuseTexture : register(t0);
SamplerState LinearSampler : register(s0);

struct VS_INPUT
{
    float4 Position : POSITION; // vertex position 
    float3 Normal : NORMAL; // vertex normal
    float2 UV : TEXCOORD0; // vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION; // vertex position 
    float2 UV : TEXCOORD0; // vertex texture coords
    float3 Normal : TEXCOORD1; // vertex normal
};

struct PS_GBUFFER_OUT 
{
    float4 ColorSpecInt : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 SpecPower : SV_TARGET2;
};

static const float2 gSpecPowerRange = { 0.1, 250.0 };

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
    VS_OUTPUT Out;
    float3 NormalWorldSpace;
    
    Out.Position = mul(input.Position, WorldViewProjection);
    
    Out.UV = input.UV;
    
    Out.Normal = mul(input.Normal, (float3x3) World);
    
    return Out;
}

PS_GBUFFER_OUT PackGBuffer(float4 BaseColor, float3 Normal, float SpecIntensity,float SpecPower)
{
    PS_GBUFFER_OUT Out;
    
    float SpecPowerNormal = (SpecPower - gSpecPowerRange.x) / gSpecPowerRange.y;
    
    Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
    Out.Normal = float4(Normal.xyz * 0.5 + 0.5, 0.0);
    Out.SpecPower = float4(SpecPowerNormal, 0.0, 0.0, 0.0);
    
    return Out;    
}

PS_GBUFFER_OUT DrawPS (VS_OUTPUT In)
{
    float3 DiffuseColor = DiffuseTexture.Sample(LinearSampler, In.UV);
    float3 Normal;
    float SpecIntensity;
    float SpecPower;
    
    return PackGBuffer(DiffuseColor, normalize(Normal), SpecIntensity, SpecPower);

}