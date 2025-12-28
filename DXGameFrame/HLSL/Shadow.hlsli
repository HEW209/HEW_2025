#ifndef INCLUDE_SHADOW
#define INCLUDE_SHADOW

#include "DXGameFrameShader.hlsli"

Texture2D<float> shadowMap : register(t8);
//Texture2D<float4> blueNoiseMap : register(t9);
SamplerComparisonState shadowSamp : register(s8);
SamplerState pointWrapSamp : register(s9);

static const float PI = 3.14159265359;

// Vogel Disk Sampling
float2 GetVogelDiskSample(int sampleIndex, int samplesCount)
{
    float GoldenAngle = 2.4; // ラジアン (約137.5度)
    
    float r = sqrt((float(sampleIndex) + 0.5) / float(samplesCount));
    float theta = float(sampleIndex) * GoldenAngle;
    
    float sine, cosine;
    sincos(theta, sine, cosine);
    
    return float2(cosine, sine) * r;
}

// 平均ブロッカー深度の検索
float FindBlocker(float2 uv, float zReceiver, float searchRegionRadiusUV)
{
    float totalBlockerDepth = 0;
    int numBlockers = 0;
    int numSearchSamples = 16; // 探索サンプル数

    for (int i = 0; i < numSearchSamples; ++i)
    {
        float2 offset = GetVogelDiskSample(i, numSearchSamples);
        float2 sampleUV = uv + offset * searchRegionRadiusUV;
        
        // シャドウマップから深度をサンプリング
        float zBlocker = shadowMap.SampleLevel(pointWrapSamp, sampleUV, 0);

        // 遮蔽物があるか
        if (zBlocker < zReceiver)
        {
            totalBlockerDepth += zBlocker;
            numBlockers++;
        }
    }

    if (numBlockers > 0)
        return totalBlockerDepth / numBlockers;
    else
        return -1.0; // ブロッカーなし
}

// PCSS計算
float CalcPCSS(float4 shadowCoord, float2 screenPos)
{
    float2 uv = shadowCoord.xy;
    float zReceiver = shadowCoord.z;
    
    if (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1 || zReceiver > 1.0)
        return 1.0;

    // ブロッカー探索
    float searchRadius = lightSize * 0.05;
    
    float avgBlockerDepth = FindBlocker(uv, zReceiver, searchRadius);

    if (avgBlockerDepth == -1.0)
        return 1.0;

    // 半影サイズの計算
    float penumbraWidth = (zReceiver - avgBlockerDepth) / avgBlockerDepth * lightSize;
    penumbraWidth = clamp(penumbraWidth, 0.0, 0.1);

    // PCFフィルタリング
    float shadow = 0.0;
    int numPCFSamples = 32;
    
    for (int i = 0; i < numPCFSamples; ++i)
    {
        float2 offset = GetVogelDiskSample(i, numPCFSamples);
        float2 sampleUV = uv + offset * penumbraWidth;
        
        shadow += shadowMap.SampleCmpLevelZero(shadowSamp, sampleUV, zReceiver);
    }

    return shadow / float(numPCFSamples);
}

#endif