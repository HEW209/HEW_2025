/******************************************************************//**
 * @file   ConfigKey.h
 * @brief  ライブラリ設定取得キー
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

/// ライブラリ設定取得キー
/// プロジェクト設定ファイルにこれらの項目が含まれているか確認してください
namespace ConfigKey
{
    /// デフォルト頂点シェーダー
    constexpr const char* DefaultVS = "DefaultVS";

    /// デフォルトピクセルシェーダー
    constexpr const char* DefaultPS = "DefaultPS";

    /// デフォルトスプライト頂点シェーダー
    constexpr const char* DefaultSpriteVS = "DefaultSpriteVS";

    /// デフォルトスプライトピクセルシェーダー
    constexpr const char* DefaultSpritePS = "DefaultSpritePS";

    /// デフォルトテクスチャ
    constexpr const char* DefaultTexture = "DefaultTexture";

    /// 1スケールに対するピクセル数 (スプライト描画に使用します)
    constexpr const char* PixelPerUnit = "PixelPerUnit";
}