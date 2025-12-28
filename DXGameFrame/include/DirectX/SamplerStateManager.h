/******************************************************************//**
 * @file   SamplerStateManager.h
 * @brief  サンプラーステートを管理する
 *
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <array>

 /// サンプラーステート
enum class SamplerState : UINT
{
	DEFAULT = 0,			// LINEAR_WRAPが使用される
	LINEAR_WRAP = 0,		// 線形補間 + 繰り返し
	LINEAR_CLAMP,			// 線形補間 + 端固定
	POINT_WRAP,				// 最近傍 + 繰り返し（ドット絵向け）
	POINT_CLAMP,			// 最近傍 + 端固定（UI向け）
	ANISOTROPIC_WRAP,		// 異方性 + 繰り返し
	ANISOTROPIC_CLAMP,		// 異方性 + 端固定
	BORDER_BLACK,			// 枠外黒
	SHADOW_COMPARISON,		// シャドウのハードウェアPCF用
	COUNT
};

/**
 * @brief サンプラーステートを管理する
 */
class SamplerStateManager
{
public:
	SamplerStateManager();
	~SamplerStateManager() = default;

	/**
	 * @brief サンプラーステートの初期化
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
	 * @brief サンプラーステートを設定する
	 * @param state 設定するサンプラーステート
	 * @param slotNum スロット番号
	 */
	void SetState(SamplerState state, UINT slotNum = 0);

private:
	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// 現在のサンプラーステート
	SamplerState m_currentState;

	/// サンプラーステート配列
	std::array<ComPtr<ID3D11SamplerState>, (size_t)SamplerState::COUNT> m_pStates;

	/**
	 * @brief 全てのサンプラーステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState();
};
