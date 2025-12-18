#ifndef INCLUDE_OUTLINE
#define INCLUDE_OUTLINE

cbuffer OutlineParam : register(b3)
{
    float4 outlineColor;
    float outlineWidth;
    float2 viewportSize;
    float pad;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
};

#endif