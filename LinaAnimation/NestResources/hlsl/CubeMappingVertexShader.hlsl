#include "../hlsl/Common.hlsli"

cbuffer VertexConstantBuffer : register(b0) { matrix viewProj;};

struct CubeMappingPixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posLocal : POSITION;
};

CubeMappingPixelShaderInput main(VertexShaderInput input)
{
    CubeMappingPixelShaderInput output;
    output.posLocal = input.posLocal;
    output.posProj = mul(float4(input.posLocal, 1.0), viewProj);
    
    return output;
}