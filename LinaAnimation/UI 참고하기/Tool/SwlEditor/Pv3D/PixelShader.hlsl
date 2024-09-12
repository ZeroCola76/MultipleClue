#include "Header.hleli"

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor,float3 Normal, float SpecIntensity, float SpecPower)
{
    PS_GBUFFER_OUT Out;
   
    float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;
    
    Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
    Out.Normal = float4(Normal.xyz * 0.5 + 0.5, 0.0);
    Out.SpecPowr = float4(SpecPowerNorm, 0.0, 0.0, 0.0);
    
    return Out;
}

PS_GBUFFER_OUT DrawPS(VS_OUTPUT In)
{
    float3 DiffuseColor;
    float3 Normal;
    float SpecIntensity;
    float SpecPower;
    
    // ���ؽ� ���̴� �Է� �� �Ǵ� �ؽ��� ���� ���� ��� �Է��� ��
    
    return PackGBuffer(DiffuseColor, normalize(Normal), SpecIntensity, SpecPower);
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}