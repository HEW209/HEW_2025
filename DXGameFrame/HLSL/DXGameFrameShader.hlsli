#ifndef INCLUDE_DXGAMEFRAME_SHADER
#define INCLUDE_DXGAMEFRAME_SHADER

// 頂点入力レイアウト
struct VS_IN
{
    float3 pos : POSITION; // 頂点座標
    float3 normal : NORMAL0; // 法線
    float2 uv : TEXCOORD0; // UV座標
    float4 color : COLOR0; // 頂点カラー
    float4 weight : WEIGHT0; // 頂点スキンウエイト
    uint4 index : INDEX0; // 対応ボーンインデックス
};

// スプライト用頂点入力レイアウト
struct VS_IN_SPRITE
{
    float2 pos : POSITION; // 頂点座標
    float2 uv : TEXCOORD0; // UV座標
};

cbuffer World : register(b0)
{
    float4x4 world; // ワールド行列
    float4x4 invWorld; // ワールド逆行列
};

cbuffer PerFrame : register(b1)
{
    float4x4 view; // ビュー行列
    float4x4 projection; // プロジェクション行列
    float3 cameraPos; // カメラ座標
    float time; // 経過時間
    float3 lightDir; // ライト方向
    float lightIntensity; // ライトの強さ
    float3 lightColor; // ライト色
    float pad1;
    float3 ambientColor; // 環境光
    float pad2;
};

#endif