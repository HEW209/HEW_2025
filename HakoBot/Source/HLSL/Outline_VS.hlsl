#include "DXGameFrameShader.hlsli"
#include "Outline.hlsli"

GS_IN main(VS_IN input)
{
    GS_IN output = (GS_IN) 0;
    
    output.pos = float4(input.pos, 1);

    // –@ü‚ğƒrƒ…[‹óŠÔ‚Ö•ÏŠ·
    float3 worldNormal = mul((float3x3) invWorld, input.normal);
    float3 worldSmooth = mul((float3x3) invWorld, input.color.rgb);
    output.normal = mul(worldNormal, (float3x3)view);
    output.smoothNormal = mul(worldSmooth, (float3x3)view);

    return output;
}