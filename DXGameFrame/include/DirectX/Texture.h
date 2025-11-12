/*****************************************************************//**
 * @file   Texture.h
 * @brief  テクスチャ情報を扱う
 * 
 * @author 石田怜
 * @date   2025/09/16
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <string>

/// テクスチャのスロット番号
namespace TextureSlot
{
	constexpr UINT Count = 8;		// 使用可能スロット数
	constexpr UINT Main = 0;		// メインテクスチャ
}

/**
 * @brief テクスチャ情報を扱う
 */
class Texture
{
public:
	Texture();
	~Texture() = default;

	/**
	 * @brief テクスチャを読み込む
	 * @param filePath テクスチャ画像へのファイルパス
	 * @return 成功したかを返す
	 */
	HRESULT Load(const std::string& filePath);

	/**
	 * @brief テクスチャをシェーダーにセットする
	 * @param slot 設定するスロット番号
	 */
	void Bind(UINT slot = TextureSlot::Main);
	
private:
	/// シェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
};
