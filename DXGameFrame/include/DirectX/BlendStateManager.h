/******************************************************************//**
 * @file   BlendStateManager.h
 * @brief  ブレンドステートを管理する
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <array>

/// ブレンドステート
enum class BlendState : UINT
{
	DEFAULT,		// ブレンド無し
	ALPHA,			// 通常のアルファブレンド
	ADD,			// 加算
	SUB,			// 減算
	MUL,			// 乗算
	DISABLE,		// カラー描画無し
	COUNT			// ブレンドモードの種類数
};

/**
 * @brief ブレンドステートを管理する
 */
class BlendStateManager
{
public:
	BlendStateManager();
	~BlendStateManager() = default;

	/**
	 * @brief ブレンドステートの初期化
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
	 * @brief ブレンドステートを設定する
	 * @param state 設定するブレンドステート
	 */
	void SetState(BlendState state);

private:
	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// 現在のブレンドステート
	BlendState m_currentState;

	/// ブレンドステート配列
	std::array<ComPtr<ID3D11BlendState>, (size_t)BlendState::COUNT> m_pStates;

	/**
	 * @brief 全てのブレンドステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState();
};
