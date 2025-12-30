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

// ホワイトノイズ／ハッシュ値の生成
static float hash(float3 p)
{
    p = frac(p * 0.3183099 + 0.1);
    p *= 17.0;
    return frac(p.x * p.y * p.z * (p.x + p.y + p.z));
}

// パーリンノイズ：擬似乱数勾配ベクトル関連
static float grad(float3 ip, float3 fp)
{
    float h = hash(ip);
    float theta = h * 6.2831853;
    float z = h * 2.0 - 1.0;
    float r = sqrt(max(0.0, 1.0 - z * z));
    float3 g = float3(r * cos(theta), r * sin(theta), z);
    return dot(g, fp);
}

// パーリンノイズ：格子の切れ目を補間で滑らかにする
float perlin3D(float3 p)
{
    float3 pi = floor(p);
    float3 pf = p - pi;

	// セルの角
    float n000 = grad(pi + float3(0, 0, 0), pf - float3(0, 0, 0));
    float n100 = grad(pi + float3(1, 0, 0), pf - float3(1, 0, 0));
    float n010 = grad(pi + float3(0, 1, 0), pf - float3(0, 1, 0));
    float n110 = grad(pi + float3(1, 1, 0), pf - float3(1, 1, 0));
    float n001 = grad(pi + float3(0, 0, 1), pf - float3(0, 0, 1));
    float n101 = grad(pi + float3(1, 0, 1), pf - float3(1, 0, 1));
    float n011 = grad(pi + float3(0, 1, 1), pf - float3(0, 1, 1));
    float n111 = grad(pi + float3(1, 1, 1), pf - float3(1, 1, 1));

	// ブロック境界を滑らかにするための補間
    float nx00 = lerp(n000, n100, pf.x);
    float nx10 = lerp(n010, n110, pf.x);
    float nx01 = lerp(n001, n101, pf.x);
    float nx11 = lerp(n011, n111, pf.x);

    float nxy0 = lerp(nx00, nx10, pf.y);
    float nxy1 = lerp(nx01, nx11, pf.y);

    float nxyz = lerp(nxy0, nxy1, pf.z);

    return nxyz;
}

// fBm3Dパーリンノイズ
float fBmPerlin3D(float3 p, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amp = 1.0;
    float3 freqP = p;

    for (int i = 0; i < octaves; i++)
    {
        sum += perlin3D(freqP) * amp;

        freqP *= lacunarity; // 周波数を増やす
        amp *= gain; // 振幅を減らす（粗さ）
    }

    return sum;
}

// 発光
float3 Emission(float3 color, float strength)
{

    return color * strength;
}

// 色を係数で混ぜる
float4 ShaderMix(float4 color1, float4 color2, float mix)
{
    return color1 * (1 - mix) + color2 * mix;
}

// フレネル効果
float Fresnel(float3 normal, float3 viewDir, float power)
{
    float ndot = dot(normalize(normal), normalize(viewDir));
    return pow((1.0 - saturate(abs(ndot))), power);
}

// HDR→LDR変換
float3 ToneMap(float3 hdr)
{
    // Filmic Curve
    const float A = 0.22;
    const float B = 0.30;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.01;
    const float F = 0.30;

    // 色変換
    float3 color = ((hdr * (A * hdr + C * B) + D * E) / (hdr * (A * hdr + B) + D * F)) - E / F;
    color = saturate(color);
    
    // 白飛び処理
    float maxc = max(color.r, max(color.g, color.b));
    float desat = saturate((maxc - 0.5) * 2.0);
    color = lerp(color, float3(1, 1, 1), desat);

    return color;
}

float4 main(PS_IN pin) : SV_TARGET
{   
    float4 outColor = baseColor; // 出力カラー
    const float4 transparentColor = float4(baseColor.rgb, 0.5); // 透過色
    
    const float noiseScale = 1; // ノイズのスケール
    const float noiseEmi = 15; // ノイズによる発光の強さ
    const float waveScale = -50; // 波のスケール
    const float scrollSpeed = 5; // 波のスクロール速度
    const float fresnelPower = 5; // フレネル効果の強さ
    const float fresnelEmi = 30; // フレネル効果による発光の強さ
    
    // fBmパーリンノイズを求める
    float noiseVal = fBmPerlin3D(pin.wPos.xyz * noiseScale, 4, 1.2, 0.2);
    noiseVal = (noiseVal + 1) * 0.5;
    
    // ノイズで発光させる
    outColor.rgb = Emission(outColor.rgb, noiseVal * noiseEmi);
    
    // 波を作成
    float waveRad = pin.wPos.y * waveScale + time * scrollSpeed;
    float wave = sin(waveRad);
    wave = (wave + 1) * 0.5;
    
    // 波を使用して透過カラーと合成
    outColor = ShaderMix(outColor, transparentColor, wave);
    
    // フレネル効果を求める
    float3 viewDir = normalize(cameraPos - pin.wPos.xyz);
    float fresnel = Fresnel(pin.normal, viewDir, fresnelPower);

    // 輪郭を発光させる
    outColor.rgb = Emission(outColor.rgb, (fresnel * fresnelEmi) + 1);
    
    // 向きチェック
    float ndot = dot(normalize(pin.normal), normalize(viewDir));
    ndot = ndot * 0.9 + 0.5;
    ndot = saturate(ndot);

    // 裏面を薄くする
    float brightness = lerp(1, -0.4, ndot);
    outColor.a = outColor.a * brightness;
    
    // 色補正
    outColor.rgb = ToneMap(outColor.rgb); // HDR
    outColor.rgb = pow(outColor.rgb, 3); // 彩度
    
    return outColor;
}