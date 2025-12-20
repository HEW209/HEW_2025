#ifndef INCLUDE_OUTLINE
#define INCLUDE_OUTLINE

cbuffer OutlineParam : register(b3)
{
    float4 outlineColor;
    float outlineWidth;
    float2 viewportSize;
    float pad;
};

struct GS_IN
{
    float4 pos : SV_POSITION;         // ローカル座標
    float3 normal : NORMAL;           // 面法線
    float3 smoothNormal : TEXCOORD1;  // スムース法線
};

struct PS_IN
{
    float4 pos : SV_POSITION;
};

#endif