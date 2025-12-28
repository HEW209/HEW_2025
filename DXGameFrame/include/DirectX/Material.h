/******************************************************************//**
 * @file   Material.h
 * @brief  マテリアルクラス
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PipelineStateManager.h"
#include <vector>
#include <array>
#include <memory>

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
	 * @brief ピクセルシェーダーをnullptrにする
	 */
	void ClearPixelShader();

	/**
	 * @brief テクスチャ画像を設定する
	 * @param filePath テクスチャ画像へのファイルパス
	 * @param slot 設定するスロット番号
	 */
	void SetTexture(const std::string& filePath, UINT slot = TextureSlot::Main);

	/**
	 * @brief マテリアルのパラメータを設定する
	 * @param data 設定するデータへのポインタ (256バイト以下)
	 * @param size 設定するデータのメモリサイズ
	 */
	void SetParameter(const void* pData, UINT size);

	/**
	 * @brief パイプラインステートを設定する
	 * @param pipelineState パイプラインステート
	 */
	void SetPipelineState(PipelineState pipelineState);

	/**
	 * @brief ラスタライザーステートを設定する
	 * @param rasterizerState ラスタライザーステート
	 */
	void SetRasterizerState(RasterizerState rasterizerState);

	/**
	 * @brief 深度ステンシルステートを設定する
	 * @param depthStencilState 深度ステンシルステート
	 */
	void SetDepthStencilState(DepthStencilState depthStencilState);

	/**
	 * @brief サンプラーステートを設定する
	 * @param samplerState サンプラーステート
	 */
	void SetSamplerState(SamplerState samplerState);

	/**
	 * @brief ブレンドステートを設定する
	 * @param blendState ブレンドステート
	 */
	void SetBlendState(BlendState blendState);

	/**
	 * @brief テクスチャを取得する
	 * @param slot テクスチャスロット番号
	 * @return テクスチャへのポインタ
	 */
	Texture* GetTexture(UINT slot = TextureSlot::Main);

private:
	/// 頂点シェーダーへのポインタ
	std::shared_ptr<VertexShader> m_pVS;

	/// ピクセルシェーダーへのポインタ
	std::shared_ptr<PixelShader> m_pPS;

	/// テクスチャ配列
	std::array<std::shared_ptr<Texture>, TextureSlot::Count> m_pTextures;

	/// マテリアルごとのパラメータ
	std::vector<BYTE> m_customParameter;

	/// パイプラインステート
	PipelineState m_pipelineState;
};