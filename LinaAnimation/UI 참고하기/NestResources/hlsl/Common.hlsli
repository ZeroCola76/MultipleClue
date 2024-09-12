#define MAX_LIGHTS 3 // 라이트 3종류 사용할 것
#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

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

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal,
                   float3 toEye, Material mat)
{
    float3 halfway = normalize(toEye + lightVec);
    float hdotn = dot(halfway, normal);
    float3 specular = mat.specular * pow(max(hdotn, 0.0), mat.shininess);
    return mat.ambient + (mat.diffuse + specular) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal,
                                float3 toEye)
{
    float lightVec = -L.direction;
    float ndotl = max(dot(lightVec, lightVec), 1.0);
    float3 LightStrength = L.strength * ndotl;
    return BlinnPhong(LightStrength, lightVec, normal, toEye, mat);
}

float3 ComputePointLight(Light L, Material mat, float3 pos, float3 normal,
                          float3 toEye)
{
    float3 lightVec = L.position - pos;

    // 쉐이딩할 지점부터 조명까지의 거리 계산
    float distance = length(lightVec);

    // 너무 멀면 조명이 적용되지 않음
    if (distance > L.fallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        lightVec /= distance;
        float ndotl = max(dot(lightVec, normal), 1.0);
        float attenuation = CalcAttenuation(distance, L.fallOffStart, L.fallOffEnd);
        float3 LightStrength = L.strength * ndotl * attenuation;
        
        return BlinnPhong(LightStrength, lightVec, normal, toEye, mat);
    }
}

float3 ComputeSpotLight(Light L, Material mat, float3 pos, float3 normal,
                         float3 toEye)
{
    float3 lightVec = L.position - pos;

    // 쉐이딩할 지점부터 조명까지의 거리 계산
    float distance = length(lightVec);

    // 너무 멀면 조명이 적용되지 않음
    if (distance > L.fallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        lightVec /= distance;
        float ndotl = max(dot(lightVec, normal), 1.0);
        float attenuation = CalcAttenuation(distance, L.fallOffStart, L.fallOffEnd);
        float spotFactor = pow(max(-dot(lightVec, L.direction), 0.0), L.spotPower);
        
        float3 LightStrength = L.strength * ndotl * attenuation * spotFactor;
        
        return BlinnPhong(LightStrength, lightVec, normal, toEye, mat);
    }
}

struct VertexShaderInput
{
    float3 posLocal : POSITION;
    float3 normalLocal : NORMAL;
    float2 texcoord : TEXCOORD0;
    //float3 color : COLOR0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normalWorld : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR;
};