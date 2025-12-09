/******************************************************************//**
 * @file   RasterizerStateManager.h
 * @brief  ラスタライザーステートを管理する
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <array>

/// ラスタライザーステート
enum class RasterizerState : UINT
{
	DEFAULT,        // デフォルト (前面描画)
	BACK,           // 背面描画
	NONE,           // 両面描画
	SCISSOR,		// シザー矩形を利用する(両面描画)
	WIREFRAME,      // ワイヤーフレーム描画
	COUNT
};

/**
 * @brief ラスタライザーステートを管理する
 */
class RasterizerStateManager
{
public:
	RasterizerStateManager();
	~RasterizerStateManager() = default;

	/**
	 * @brief ラスタライザーステートの初期化
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
	 * @brief ラスタライザーステートを設定する
	 * @param state 設定するラスタライザーステート
	 */
	void SetState(RasterizerState state);

private:
	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// 現在のラスタライザーステート
	RasterizerState m_currentState;

	/// ラスタライザーステート配列
	std::array<ComPtr<ID3D11RasterizerState>, (size_t)RasterizerState::COUNT> m_pStates;

	/**
	 * @brief 全てのラスタライザーステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState();
};