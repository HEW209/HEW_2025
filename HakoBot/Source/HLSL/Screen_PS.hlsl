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

cbuffer Param : register(b3)
{
    float4 baseColor;
}

// 発光
float3 Emission(float3 color, float strength)
{
    return color * strength;
}

// フレネル効果
float Fresnel(float3 normal, float3 viewDir, float power)
{
    float ndot = dot(normalize(normal), normalize(viewDir));
    return pow((1.0 - saturate(abs(ndot))), power);
}

// HDR→LDR変換
float3 ToneMap(float3 hdr)
{
    // Filmic Curve
    const float A = 0.22;
    const float B = 0.30;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.01;
    const float F = 0.30;

    // 色変換
    float3 color = ((hdr * (A * hdr + C * B) + D * E) / (hdr * (A * hdr + B) + D * F)) - E / F;
    color = saturate(color);
    
    // 白飛び処理
    float maxc = max(color.r, max(color.g, color.b));
    float desat = saturate((maxc - 0.5) * 2.0);
    color = lerp(color, float3(1, 1, 1), desat);

    return color;
}

float3 SaturationBoost(float3 color, float boost)
{
    float luma = dot(color, float3(0.299, 0.587, 0.114));
    return lerp(luma.xxx, color, boost);
}

// 環境色（空色）
// わざと少し青を入れることで「プラスチック反射」
static const float3 envColor = float3(0.75, 0.85, 1.0);
static const float3 LUMINANCE_WEIGHTS = float3(0.2126f, 0.7152f, 0.0722f);

float4 main(PS_IN pin) : SV_TARGET
{
    float3 albedo = baseColor.rgb; // 出力カラー
    
    const float fresnelPower = 5; // フレネル効果の強さ
    const float fresnelEmi = 30; // フレネル効果による発光の強さ
    
    const float contrastThreshold = 0.15; // コントラスト閾値
    const float contrast = 2.3; // コントラスト強度
    
    const float emitThreshold = 0.1; // 発光閾値
    const float emitIntensity = 2; // 発光強度
    
    float3 viewDir = normalize(cameraPos - pin.wPos.xyz);
    float fresnel = Fresnel(pin.normal, viewDir, fresnelPower);
    
    // 輪郭を発光させる
    //albedo = Emission(albedo, (fresnel * fresnelEmi) + 1);
    
    // 色補正
    albedo = ToneMap(albedo); // HDR
    
    // 現在の輝度を計算
    float lum = dot(albedo, LUMINANCE_WEIGHTS);

    // 輝度に対してコントラスト計算
    float newLum = (lum - contrastThreshold) * contrast + contrastThreshold;
    
    albedo = albedo * saturate(newLum / (lum + 0.0001f));

    albedo = saturate(albedo);
    
    // エミッシブ計算
    lum = dot(albedo, LUMINANCE_WEIGHTS);
    
    float emitFactor = smoothstep(emitThreshold, emitThreshold + 0.3f, lum);

    // 発光色を計算
    float3 glow = albedo * emitIntensity;

    float2 absUV = 1.0 - abs(pin.uv - 0.5f) * 2.0f;
    float centerDist = min(absUV.x, absUV.y);
    float glowIntensity = smoothstep(0.0, 0.7, centerDist) * 0.9 + 0.1;

    glow *= glowIntensity * emitFactor;
    
    albedo *= centerDist * 0.5 + 0.5;
    // 元の色に加算する
    albedo += glow;
    
    float gloss = 64; // ハイライトのシャープさ
    float specPower = 0.5; // スペキュラ
    float reflAmount = 0.3; // 
    float rimAmount = 0.2; // リムライト

    float3 N = normalize(pin.normal);
    float3 V = normalize(cameraPos - pin.wPos.xyz);
    float3 L = normalize(lightDir);

    float3 ambient = albedo * ambientColor * 0.8;

    float NdotL = saturate(dot(N, L));
    float3 diffuse = albedo * lightColor * NdotL;

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
    float pcssShadow = CalcPCSS(shadowPos, pin.pos.xy);
    
    float3 directLight = (diffuse + specular) * pcssShadow;

    float3 finalColor = ambient + directLight + reflection + rimLight;
    
    finalColor *= 0.85;

    // ★★★ 彩度アップ処理 ★★★
    finalColor = SaturationBoost(finalColor, 1.3); // ← 彩度1.3倍
    
    finalColor = saturate(finalColor);

    return float4(finalColor, 1.0);
}