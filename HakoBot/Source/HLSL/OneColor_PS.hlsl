#include "DXGameFrameShader.hlsli"

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

float4 main() : SV_TARGET
{
	return baseColor;
}