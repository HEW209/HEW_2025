#include "DXGameFrameShader.hlsli"

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 wPos : POSITION0;
};

cbuffer Bone : register(b2)
{
    float4x4 bone[200];
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    float4x4 anime;
    anime = bone[vin.index.x] * vin.weight.x;
    anime += bone[vin.index.y] * vin.weight.y;
    anime += bone[vin.index.z] * vin.weight.z;
    anime += bone[vin.index.w] * vin.weight.w;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, anime);
    vout.pos = mul(vout.pos, world);
    vout.wPos = vout.pos;
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, projection);
    vout.normal = vin.normal;
    vout.normal = mul(vout.normal, (float3x3) anime);
    vout.normal = mul(vout.normal, (float3x3) world);
    vout.uv = vin.uv;
    vout.color = vin.color;
    return vout;
}