#include "DXGameFrameShader.hlsli"
#include "Outline.hlsli"
#include "GroupTransparent.hlsli"

float4 main(PS_IN input) : SV_Target
{
    CheckDepth(input.pos);
    
    return float4(outlineColor.rgb, outlineColor.a * transparency);
}