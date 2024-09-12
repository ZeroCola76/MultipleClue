#include "../hlsl/Common.hlsli"

Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
SamplerState g_sampler : register(s0); //2개 이상 텍스처 쓸 때 체인지용

cbuffer PixelShaderConstantBuffer : register(b0)
{
    float3 eyeWorld;
    bool useTexture;
    Material material;
    Light lights[MAX_LIGHTS];
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 toEye = normalize(eyeWorld - input.posWorld);
  
    return g_texture0.Sample(g_sampler, input.texcoord);
}
