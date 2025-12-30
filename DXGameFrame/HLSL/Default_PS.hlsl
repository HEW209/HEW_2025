#include "DXGameFrameShader.hlsli"
#include "Shadow.hlsli"

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 wPos : POSITION0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float3 SaturationBoost(float3 color, float boost)
{
    float luma = dot(color, float3(0.299, 0.587, 0.114));
    return lerp(luma.xxx, color, boost);
}

// 環境色（空色）
// わざと少し青を入れることで「プラスチック反射」
static const float3 envColor = float3(0.75, 0.85, 1.0);

float4 main(PS_IN pin) : SV_TARGET
{
    float gloss = 64;       // ハイライトのシャープさ
    float specPower = 0.5;  // スペキュラ
    float reflAmount = 0.3; // 
    float rimAmount = 0.2;  // リムライト

    float3 N = normalize(pin.normal);
    float3 V = normalize(cameraPos - pin.wPos.xyz);
    float3 L = normalize(lightDir);

    float3 albedo = tex.Sample(samp, pin.uv).rgb;

    float3 ambient = albedo * ambientColor * 0.8;

    float NdotL = saturate(dot(N, L));
    float3 diffuse = albedo * lightColor * (NdotL * 0.7 + 0.3);

    float3 H = normalize(L + V);
    float NdotH = max(dot(N, H), 0.0);
    float3 specColor = lerp(float3(1, 1, 1), albedo, 0.7); // ← 0.0=白 / 1.0=色反射
    float3 specular = pow(NdotH, gloss) * specColor * specPower;

    float3 R = reflect(-V, N);
    float upReflect = saturate(R.y * 0.5 + 0.5);
    float3 reflection = envColor * upReflect * reflAmount;

    float rim = pow(1.0 - saturate(dot(N, V)), 2.0);
    float3 rimLight = rim * rimAmount;
    
    // シャドウマップ座標への変換
    float4 shadowPos = mul(pin.wPos, lightViewProj);
    shadowPos.xyz /= shadowPos.w;
    shadowPos.xy = shadowPos.xy * float2(0.5, -0.5) + 0.5;

    // PCSS 計算
    float pcssShadow = 1.0;
    
    if (NdotL > 0.0)
    {
        pcssShadow = CalcPCSS(shadowPos, pin.pos.xy);
    }
    
    float3 directLight = (diffuse + specular) * pcssShadow;

    float3 finalColor = ambient + directLight + reflection + rimLight;
    
    finalColor *= 0.85;

    // ★★★ 彩度アップ処理 ★★★
    finalColor = SaturationBoost(finalColor, 1.3); // ← 彩度1.3倍

    return float4(finalColor, 1.0);
}