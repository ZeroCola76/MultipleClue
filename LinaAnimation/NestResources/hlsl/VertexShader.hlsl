#include "../hlsl/Common.hlsli"

cbuffer WorldViewProjectionConstantBuffer : register(b0) {
    matrix world;
    matrix invTranspose;
    matrix view;
    matrix projection;
    matrix trsMatrix;
    matrix BoneTransforms[MAX_MODEL_TRANSFORMS]; //김예리나가 추가함
    TweenFrameDesc TweenFrames;
    Texture2DArray TransformMap;
    uint padding[3];
};

//main이름이 잘못된것같은가? 
PixelShaderInput Animation_Main(VertexShaderInput input) 
{
    PixelShaderInput output;

    float4 posValue = float4(input.posLocal, 1.0f);
    
    float4x4 matWorld;
    
    matWorld = mul(input.blendWeights.x, BoneTransforms[input.blendIndices.x]);
    matWorld += mul(input.blendWeights.y, BoneTransforms[input.blendIndices.y]);
    matWorld += mul(input.blendWeights.z, BoneTransforms[input.blendIndices.z]);
    matWorld += mul(input.blendWeights.w, BoneTransforms[input.blendIndices.w]);
    //posValue = mul(posValue, BoneTransforms[BoneIndex]); //이건 구라핑
    posValue = mul(posValue, matWorld); //본이 있는 메쉬
    //posValue = mul(posValue, world); //본이 없는 메쉬만 할떄
    
    output.posWorld = posValue.xyz; // 월드 위치 따로 저장

    //posValue = mul(posValue, trsMatrix);
    posValue = mul(posValue, view);
    output.posProj = mul(posValue, projection);

    output.texcoord = input.texcoord;

    float4 normalvalue = float4(input.normalLocal, 0.0f);
    output.normalWorld = mul(normalvalue, invTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);

    return output;
}