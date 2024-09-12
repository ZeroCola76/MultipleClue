#define MAX_LIGHTS 3 // 라이트 3종류 사용할 것
#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 1
#define NUM_SPOT_LIGHTS 1
#define MAX_MODEL_TRANSFORMS 250 //본의 행렬을 저장

// 재질
struct Material
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    float dummy1;
    float3 specular;
    float dummy2;
};

// 조명
struct Light
{
    float3 strength;
    float fallOffStart;
    float3 direction;
    float fallOffEnd;
    float3 position;
    float spotPower;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

struct VertexShaderInput
{
    float3 posLocal : POSITION;
    float3 normalLocal : NORMAL;
    float2 texcoord : TEXCOORD0;
    float4 blendIndices : BLEND_INDICES;
    float4 blendWeights : BLEND_WEIGHTS;
    //matrix world : INST;
    //float3 color : COLOR0;
    //여기에 나중에 본인덱스4개 uint4 로 받도록  float4로 가중치를 받도록
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR;
};

struct KeyframeDesc
{
    int animIndex;
    uint currFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

struct TweenFrameDesc
{
    float tweenDuration;
    float tweenRatio;
    float tweenSumTime;
    float padding;
    KeyframeDesc curr;
    KeyframeDesc next;
};
