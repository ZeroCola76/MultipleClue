#include "../hlsl/Common.hlsli"

cbuffer WorldViewProjectionConstantBuffer : register(b0) {
    matrix world;
    matrix invTranspose;
    matrix view;
    matrix projection;
};

PixelShaderInput main(VertexShaderInput input) {

    PixelShaderInput output;
    float4 pos = float4(input.posLocal, 1.0f);
    pos = mul(pos, world);
    
    output.posWorld = pos.xyz; // 월드 위치 따로 저장
    
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.texcoord = input.texcoord;
    //output.color = input.color;

    float4 normal = float4(input.normalLocal, 0.0f);
    output.normalWorld = mul(normal, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    
    return output;
}
