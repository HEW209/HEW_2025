/*****************************************************************//**
 * @file   Material.h
 * @brief  マテリアル基底クラス
 * 
 * @author 石田怜
 * @date   2025/09/21
 *********************************************************************/
#pragma once

#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <Utility/Common.h>

/**
 * @brief マテリアル基底クラス
 */
class Material
{
public:
	Material() = default;
	virtual ~Material() = default;

	/**
	 * @brief マテリアルをセットする
	 */
 	virtual void Bind() = 0;

	/**
	 * @brief メインテクスチャを設定する
	 * @param filePath テクスチャ画像へのファイルパス
	 */
	void SetTexture(const std::string& filePath);

protected:
	/// メインテクスチャへのポインタ
	std::shared_ptr<Texture> m_pMainTexture;

	/// 頂点シェーダーへのポインタ
	std::shared_ptr<VertexShader> m_pVS;

	/// ピクセルシェーダーへのポインタ
	std::shared_ptr<PixelShader> m_pPS;
};