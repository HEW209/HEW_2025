/*****************************************************************//**
 * @file   PipelineState.h
 * @brief  全ての描画ステートを扱う
 * 
 * @author 石田怜
 * @date   2025/10/10
 *********************************************************************/
#pragma once

#include <DirectX/RasterizerState.h>
#include <DirectX/DepthStencilState.h>
#include <DirectX/BlendState.h>
#include <DirectX/SamplerState.h>

/**
 * @brief 全ての描画ステートを扱う
 */
class PipelineState
{
public:
	/**
	 * @brief パイプラインステートの初期化
	 * @param pDevice D3Dデバイスへのポインタ
	 * @param pContext D3Dデバイスコンテキストへのポインタ
	 * @return 成功したかを返す
	 */
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/**
	 * @brief 終了処理
	 */
	void Uninit();

	/**
	 * @brief 全ての描画ステートモードを設定する
	 * @param rasterizerMode ラスタライズモード
	 * @param depthStencilMode 深度ステンシルモード
	 * @param blendMode ブレンドモード
	 * @param samplerMode サンプラーモード (0番目のスロットに設定)
	 */
	void SetPipelineMode(
		RasterizerState::Mode rasterizeMode,
		DepthStencilState::Mode depthStencilMode,
		BlendState::Mode blendMode,
		SamplerState::Mode samplerMode
	);

	/**
	 * @brief ラスタライズモードを設定する
	 * @param rasterizeMode 設定するラスタライズモード
	 */
	void SetRasterizeMode(RasterizerState::Mode rasterizeMode);

	/**
	 * @brief 深度ステンシルモードを設定する
	 * @param depthStencilMode 設定する深度ステンシルモード
	 */
	void SetDepthStencilMode(DepthStencilState::Mode depthStencilMode);

	/**
	 * @brief ブレンドモードを設定する
	 * @param blendMode 設定するブレンドモード
	 */
	void SetBlendMode(BlendState::Mode blendMode);

	/**
	 * @brief サンプラーモードを設定する
	 * @param samplerMode 設定するサンプラーモード
	 * @param slotNum スロット番号
	 */
	void SetSamplerMode(SamplerState::Mode samplerMode, UINT slotNum = 0);

private:
	PipelineState() = default;

	/// ラスタライザーステート
	RasterizerState m_rasterizerState;

	/// 深度ステンシルステート
	DepthStencilState m_depthStencilState;

	/// ブレンドステート
	BlendState m_blendState;

	/// サンプラーステート
	SamplerState m_samplerState;

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return PipelineStateへの参照
	 */
	static PipelineState& Instance()
	{
		static PipelineState s_instance;
		return s_instance;
	}
};