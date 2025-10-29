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
	 */
	void Load(const std::string& filePath);

	/**
	 * @brief テクスチャをシェーダーにセットする
	 * @param slot 設定するスロット番号
	 */
	void Bind(UINT slot = 0);
	
private:
	/// シェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
};
