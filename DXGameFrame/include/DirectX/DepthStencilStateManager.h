/******************************************************************//**
 * @file   DepthStencilStateManager.h
 * @brief  深度ステンシルステートを管理する
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <array>

/// 深度ステンシルステート
enum class DepthStencilState : UINT
{
	DEFAULT,		// 深度テストON 書き込みON (通常)
	READ_ONLY,		// 深度テストON 書き込みOFF (半透明用)
	DISABLE,		// 深度テストOFF (UI・2D用)
	STENCIL,		// ステンシルON
	DISABLE_STENCIL,// 深度テストOFF、ステンシルON
	OUTLINE,		// アウトライン描画用
	OUTLINE_READONLY,	// アウトライン描画用（深度書き込みOFF）
	COUNT
};

/**
 * @brief 深度ステンシルステートを管理する
 */
class DepthStencilStateManager
{
public:
	DepthStencilStateManager();
	~DepthStencilStateManager() = default;

	/**
	 * @brief 深度ステンシルステートの初期化
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
	 * @brief 深度ステンシルステートを設定する
	 * @param state 設定する深度ステンシルステート
	 */
	void SetState(DepthStencilState state);

private:
	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// 現在の深度ステンシルステート
	DepthStencilState m_currentState;

	/// 深度ステンシルステート配列
	std::array<ComPtr<ID3D11DepthStencilState>, (size_t)DepthStencilState::COUNT> m_pStates;

	/**
	 * @brief 全ての深度ステンシルステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState();
};