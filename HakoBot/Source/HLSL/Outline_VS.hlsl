#include "DXGameFrameShader.hlsli"
#include "Outline.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    // クリップ空間へ変換
    float4 worldPos = mul(float4(input.pos.xyz, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.pos = mul(viewPos, projection);

    // 法線をビュー空間に変換
    float3 viewNormal = mul(input.normal, (float3x3) invWorld);
    
    // 正規化
    viewNormal = normalize(viewNormal);

    // 画面上の押し出し方向を計算
    float2 offsetDir = normalize(viewNormal.xy);

    if (length(viewNormal.xy) < 0.0001f)
    {
        offsetDir = float2(0, 0);
    }

    // ピクセル単位の押し出し計算
    float2 pixelScale = float2(2.0f / viewportSize.x, 2.0f / viewportSize.y);
    
    output.pos.xy += offsetDir * outlineWidth * pixelScale * output.pos.w;

    // 深度バイアス
    output.pos.z += 0.00001f * output.pos.w;

    return output;
}