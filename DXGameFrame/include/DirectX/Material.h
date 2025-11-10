/*****************************************************************//**
 * @file   Material.h
 * @brief  マテリアルクラス
 * 
 * @author 石田怜
 * @date   2025/09/21
 *********************************************************************/
#pragma once

#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PipelineState.h"
#include <Utility/Common.h>

constexpr UINT MaxTextureSlot = 8;		// テクスチャスロット数
constexpr UINT MainTextureSlot = 0;		// メインテクスチャのスロット番号

/**
 * @brief マテリアルクラス
 */
class Material
{
public:
	Material();
	virtual ~Material() = default;

	/**
	 * @brief マテリアルをセットする
	 */
 	void Bind() const;

	/**
	 * @brief 頂点シェーダーを設定する
	 * @param filePath 頂点シェーダー(.cso)へのファイルパス
	 */
	void SetVertexShader(const std::string& filePath);

	/**
	 * @brief ピクセルシェーダーを設定する
	 * @param filePath ピクセルシェーダー(.cso)へのファイルパス
	 */
	void SetPixelShader(const std::string& filePath);

	/**
	 * @brief テクスチャ画像を設定する
	 * @param filePath テクスチャ画像へのファイルパス
	 * @param slot 設定するスロット番号
	 */
	void SetTexture(const std::string& filePath, UINT slot = 0);

	/**
	 * @brief マテリアルのパラメータを設定する
	 * @param data 設定するデータへのポインタ (256バイト以下)
	 * @param size 設定するデータのメモリサイズ
	 */
	void SetParameter(const void* data, UINT size);

	/**
	 * @brief パイプラインステートモードを設定する
	 * @param pipelineMode パイプラインステートごとのモード設定
	 */
	void SetPipelineMode(PipelineState::ModeSet pipelineMode);

private:
	/// 頂点シェーダーへのポインタ
	std::shared_ptr<VertexShader> m_pVS;

	/// ピクセルシェーダーへのポインタ
	std::shared_ptr<PixelShader> m_pPS;

	/// テクスチャ情報
	std::shared_ptr<Texture> m_pTextures[MaxTextureSlot];
	
	/// マテリアルごとのパラメータ
	std::vector<BYTE> m_customParameter;

	/// パイプラインステートのモード
	PipelineState::ModeSet m_pipelineMode;
};