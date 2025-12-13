/******************************************************************//**
 * @file   PipelineStateManager.h
 * @brief  全ての描画ステートを管理する
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "RasterizerStateManager.h"
#include "DepthStencilStateManager.h"
#include "SamplerStateManager.h"
#include "BlendStateManager.h"

/// パイプラインステート
struct PipelineState
{
	/// ラスタライザーステート
	RasterizerState rasterizerState = RasterizerState::DEFAULT;

	/// 深度ステンシルステート
	DepthStencilState depthStencilState = DepthStencilState::DEFAULT;

	/// サンプラーステート
	SamplerState samplerState = SamplerState::DEFAULT;

	/// ブレンドステート
	BlendState blendState = BlendState::DEFAULT;
};

/**
 * @brief 全ての描画ステートを管理する
 */
class PipelineStateManager
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
	 * @brief 全ての描画ステートを設定する
	 * @param rasterizerState ラスタライザーステート
	 * @param depthStencilState 深度ステンシルステート
	 * @param samplerState サンプラーステート
	 * @param blendState ブレンドステート
	 */
	void SetPipelineState(
		RasterizerState rasterizerState,
		DepthStencilState depthStencilState,
		SamplerState samplerState,
		BlendState blendState
	);

	/**
	 * @brief 全ての描画ステートステートを設定する
	 * @param pipelineState パイプラインステート
	 */
	void SetPipelineState(PipelineState pipelineState);

	/**
	 * @brief ラスタライズステートを設定する
	 * @param rasterizerState 設定するラスタライズステート
	 */
	void SetRasterizerState(RasterizerState rasterizerState);

	/**
	 * @brief 深度ステンシルステートを設定する
	 * @param depthStencilState 設定する深度ステンシルステート
	 */
	void SetDepthStencilState(DepthStencilState depthStencilState);

	/**
	 * @brief サンプラーステートを設定する
	 * @param samplerState 設定するサンプラーステート
	 * @param slotNum スロット番号
	 */
	void SetSamplerState(SamplerState samplerState, UINT slotNum = 0);

	/**
	 * @brief ブレンドステートを設定する
	 * @param blendState 設定するブレンドステート
	 */
	void SetBlendState(BlendState blendState);

	/**
	 * @brief パイプラインステートのキャッシュをリセットする
	 * PipelineManagerを介さずにステートが変更されたとき呼び出してください
	 */
	void Refresh();

private:
	PipelineStateManager() = default;

	/// ラスタライザーステートマネージャー
	RasterizerStateManager m_rasterizerStateManager;

	/// 深度ステンシルステートマネージャー
	DepthStencilStateManager m_depthStencilStateManager;

	/// ブレンドステートマネージャー
	BlendStateManager m_blendStateManager;

	/// サンプラーステートマネージャー
	SamplerStateManager m_samplerStateManager;

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return PipelineStateManagerへの参照
	 */
	static PipelineStateManager& Instance();
};
