#include "DXGameFrameShader.hlsli"

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

cbuffer Param : register(b3)
{
    float2 pos;
    float2 scale;
    float2 uvPos;
    float2 uvScale;
    float4 color;
    
    float fadeRatio;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float2 uv = pin.uv;
    float2 center = float2(0.5, 0.5);
    float2 centerTo = uv - center;
    
    // 距離マスク
    float dist = max(abs(centerTo.x), abs(centerTo.y));
    float border = 1 - fadeRatio;
    float distMask = smoothstep(border, border - 0.003, dist);
    
    // テクスチャマスク
    float2 maskUV = center + centerTo * (1 / max(1 - fadeRatio, 1e-6));
    float texMask = tex.Sample(samp, maskUV).a;

    float mask = distMask * texMask;
    pin.color.a *= 1 - mask;
    return pin.color;
}