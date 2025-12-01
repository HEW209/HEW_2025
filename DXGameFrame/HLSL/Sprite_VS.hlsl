#include "DXGameFrameShader.hlsli"

struct VS_OUT
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
    
    float dummy[208];
};

VS_OUT main(VS_IN_SPRITE vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 0.0f, 1.0f);
    vout.uv = vin.uv;
    
    // スプライトパラメータを適用
    vout.pos.xy *= scale;
    vout.pos.xy += pos;
    vout.uv *= uvScale;
    vout.uv += uvPos;
    vout.color = color;
    
    // 行列演算
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, projection);
    return vout;
}