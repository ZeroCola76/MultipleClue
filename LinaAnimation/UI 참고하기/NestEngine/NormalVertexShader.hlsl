#include "Common.hlsli"

cbuffer WorldViewProjectionConstantBuffer : register(b0)
{
    matrix world;
    matrix invTranspose;
    matrix view;
    matrix projection;
};

cbuffer NormalVertexConstantBuffer : register(b1)
{
    //matrix world;
    //matrix invTranspose;
    //matrix view;
    //matrix projection;
    float scale;
};

PixelShaderInput main(VertexShaderInput input)
{

    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f);
    
    // Normal ���� ��ȯ
    float4 normal = float4(input.normalModel, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    pos = mul(pos, world);
    
    float normalRatio = input.texcoord.x;
    
    pos.xyz += output.normalWorld * normalRatio * scale;
    
    output.posWorld = pos.xyz; // ���� ��ġ ���� ����
    
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.texcoord = input.texcoord;
    output.color = float3(1.0, 1.0, 0.0) * (1.0 - normalRatio) + float3(1.0, 0.0, 0.0) * normalRatio;
    
    return output;
}