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
#include <vector>
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
	void SetPipelineMode(PipelineState::ModeSet pipelineMode)
	{
		m_pipelineMode = pipelineMode;
	}

	/**
	 * @brief ラスタライズモードを設定する
	 * @param rasterizeMode ラスタライズモード設定
	 */
	void SetRasterizeMode(RasterizerState::Mode rasterizeMode)
	{
		m_pipelineMode.rasterizeMode = rasterizeMode;
	}

	/**
	 * @brief 深度ステンシルモードを設定する
	 * @param depthStencilMode 深度ステンシルモード設定
	 */
	void SetDepthStencilMode(DepthStencilState::Mode depthStencilMode)
	{
		m_pipelineMode.depthStencilMode = depthStencilMode;
	}

	/**
	 * @brief ブレンドモードを設定する
	 * @param blendMode ブレンドモード設定
	 */
	void SetBlendMode(BlendState::Mode blendMode)
	{
		m_pipelineMode.blendMode = blendMode;
	}

	/**
	 * @brief サンプラーモードを設定する
	 * @param samplerMode サンプラーモード設定
	 */
	void SetSamplerMode(SamplerState::Mode samplerMode)
	{
		m_pipelineMode.samplerMode = samplerMode;
	}

private:
	/// 頂点シェーダーへのポインタ
	std::shared_ptr<VertexShader> m_pVS;

	/// ピクセルシェーダーへのポインタ
	std::shared_ptr<PixelShader> m_pPS;

	/// テクスチャ配列
	std::shared_ptr<Texture> m_pTextures[TextureSlot::Count];
	
	/// マテリアルごとのパラメータ
	std::vector<BYTE> m_customParameter;

	/// パイプラインステートのモード
	PipelineState::ModeSet m_pipelineMode;
};