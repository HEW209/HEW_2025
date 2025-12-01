/******************************************************************//**
 * @file   Texture.h
 * @brief  テクスチャを扱う
 * 
 * @author 石田怜
 * @date   2025/11/22
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

	/// テクスチャサイズ
	struct Size
	{
		UINT x;		// テクスチャの幅
		UINT y;		// テクスチャの高さ
	};

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

	/**
	 * @brief このテクスチャのサイズを取得する
	 * @return テクスチャサイズ (ピクセル)
	 */
	Size GetSize();

private:
	/// シェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_pSRV;

	/// テクスチャ画像のサイズ
	Size m_size;

	/**
	 * @brief 実際のテクスチャ読み込み処理
	 * @param filePath テクスチャ画像へのファイルパス
	 * @return 成功したかを返す
	 */
	HRESULT LoadFromFile(const std::string& filePath);
};