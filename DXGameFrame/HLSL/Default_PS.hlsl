struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 wPos : POSITION0;
};

cbuffer Light : register(b1)
{
    float3 lightDir;
    float pad1;
    float3 lightColor;
    float pad2;
    float3 ambientColor;
    float pad3;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    //float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //color = tex.Sample(samp, pin.uv);
    //return color;
    
    float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir); // Œõ‚Ì‹t•ûŒü
    float NdotL = saturate(dot(N, L));
    
    // ŠÂ‹«Œõ + ŠgŽU”½ŽË
    float3 color = ambientColor + lightColor * NdotL;
    
    float4 texColor = tex.Sample(samp, pin.uv);
    return float4(texColor.rgb * color, texColor.a);
}