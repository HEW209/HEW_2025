#include "DXGameFrameShader.hlsli"
#include "Outline.hlsli"

// 押し出し後の頂点位置を計算する関数
// dir: ビュー空間での法線, pos: クリップ空間での頂点座標
#define EXTRUDE(pos, dir) (pos + float4(normalize(dir.xy) * outlineWidth * pixelScale * pos.w, 0, 0))

[maxvertexcount(21)]
void main(triangle GS_IN input[3], inout TriangleStream<PS_IN> triStream)
{
    float4 viewPos[3];
    float3 faceNormal[3];
    float3 smoothNormal[3];
    float2 pixelScale = float2(2.0f / viewportSize.x, 2.0f / viewportSize.y);

    // 座標変換
    for (int i = 0; i < 3; i++)
    {
        float4 worldPos = mul(input[i].pos, world);
        float4 vPos = mul(worldPos, view);
        viewPos[i] = mul(vPos, projection); // クリップ空間座標

        faceNormal[i] = normalize(input[i].normal);
        smoothNormal[i] = normalize(input[i].smoothNormal);
    }

    // 各頂点についてpFaceとpSmoothを計算
    float4 pFace[3]; // 面法線での押し出し位置
    float4 pSmooth[3]; // スムース法線での押し出し位置

    for (int i = 0; i < 3; i++)
    {
        // ゼロ除算対策
        float3 fn = (length(faceNormal[i].xy) < 0.0001f) ? float3(0, 0, 1) : faceNormal[i];
        float3 sn = (length(smoothNormal[i].xy) < 0.0001f) ? float3(0, 0, 1) : smoothNormal[i];

        pFace[i] = EXTRUDE(viewPos[i], fn);
        pSmooth[i] = EXTRUDE(viewPos[i], sn);
        
        // Zファイティング対策
        //pFace[i].z += 0.00001f * pFace[i].w;
        //pSmooth[i].z += 0.00001f * pSmooth[i].w;
    }

    // 本体ポリゴンの描画
    PS_IN v;
    
    v.pos = pFace[0];
    triStream.Append(v);
    v.pos = pFace[1];
    triStream.Append(v);
    v.pos = pFace[2];
    triStream.Append(v);
    triStream.RestartStrip();
    
    // ブリッジポリゴンの描画
    // 各エッジについて、face[i], smooth[i], smooth[next], face[next]) を結ぶ四角形を作る
    
    for (int i = 0; i < 3; i++)
    {
        int next = (i + 1) % 3;

        // 四角形を2つの三角形として出力
        // 三角形1: face[i] -> smooth[i] -> face[next]
        v.pos = pFace[i];
        triStream.Append(v);
        v.pos = pSmooth[i];
        triStream.Append(v);
        v.pos = pFace[next];
        triStream.Append(v);
        triStream.RestartStrip();

        // 三角形2: smooth[i] -> smooth[next] -> face[next]
        v.pos = pSmooth[i];
        triStream.Append(v);
        v.pos = pSmooth[next];
        triStream.Append(v);
        v.pos = pFace[next];
        triStream.Append(v);
        triStream.RestartStrip();
    }
}