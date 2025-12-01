/******************************************************************//**
 * @file   TextureManager.h
 * @brief  テクスチャを管理する
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "Texture.h"
#include <unordered_map>
#include <memory>
#include <vector>

/**
 * @brief テクスチャを管理する
 */
class TextureManager
{
public:
	/**
	 * @brief 多重読み込みを防ぎつつテクスチャを読み込む
	 * @param filePath テクスチャ画像へのファイルパス
	 * @return テクスチャへのポインタ(shared_ptr)
	 */
	std::shared_ptr<Texture> LoadTexture(const std::string& filePath);

	/**
	 * @brief 使用していないアセットを解放する
	 */
	void CollectGarbage();

	/**
	 * @brief 全てのテクスチャを解放する
	 */
	void Clear();

private:
	TextureManager() = default;

	/// テクスチャとファイルパスのマップ
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return TextureManagerインスタンスへの参照
	 */
	static TextureManager& Instance();
};
