#define MAX_LIGHTS 3 // ����Ʈ 3���� ����� ��
#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

// ����
struct Material
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    float dummy1;
    float3 specular;
    float dummy2;
};

// ����
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

    // ���̵��� �������� ��������� �Ÿ� ���
    float distance = length(lightVec);

    // �ʹ� �ָ� ������ ������� ����
    if (distance > L.fallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        lightVec /= distance;
        float ndotl = max(dot(lightVec, normal), 1.0f);
        float attenuation = CalcAttenuation(distance, L.fallOffStart, L.fallOffEnd);
        float3 LightStrength = L.strength * ndotl * attenuation;
        
        return BlinnPhong(LightStrength, lightVec, normal, toEye, mat);
    }
}

float3 ComputeSpotLight(Light L, Material mat, float3 pos, float3 normal,
                         float3 toEye)
{
    float3 lightVec = L.position - pos;

    // ���̵��� �������� ��������� �Ÿ� ���
    float distance = length(lightVec);

    // �ʹ� �ָ� ������ ������� ����
    if (distance > L.fallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        lightVec /= distance;
        float ndotl = max(dot(lightVec, normal), 1.0f);
        float attenuation = CalcAttenuation(distance, L.fallOffStart, L.fallOffEnd);
        float spotFactor = pow(max(-dot(lightVec, L.direction), 0.0f), L.spotPower);
        
        float3 LightStrength = L.strength * ndotl * attenuation * spotFactor;
        
        return BlinnPhong(LightStrength, lightVec, normal, toEye, mat);
    }
}

struct VertexShaderInput
{
    float3 posModel : POSITION;
    float3 normalModel : NORMAL;
    //float3 color : COLOR0;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normalWorld : NORMAL;
    //float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};