#include "DXGameFrameShader.hlsli"
#include "Outline.hlsli"

float4 main(PS_IN input) : SV_Target
{
    return outlineColor;
}