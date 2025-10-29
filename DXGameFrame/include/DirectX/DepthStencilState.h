/*****************************************************************//**
 * @file   DepthStencilState.h
 * @brief  深度ステンシルステートを扱う
 * 
 * @author 石田怜
 * @date   2025/09/25
 *********************************************************************/
#pragma once

#include <DirectX/StateBase.h>

/**
 * @brief 深度ステンシルステートを扱う
 */
class DepthStencilState : public StateBase
{
public:
	DepthStencilState();
	~DepthStencilState() = default;

	/// 深度ステンシルモード
	enum Mode
	{
		DEFAULT,		//深度テストON 書き込みON (通常)
		READ_ONLY,		//深度テストON 書き込みOFF (半透明用)
		DISABLE,		//深度テストOFF (UI・2D用)
		STENCIL,		//ステンシルON
		COUNT
	};

	/**
	 * @brief 終了処理
	 */
	void Uninit() final;

	/**
	 * @brief 深度ステンシルモードを設定する
	 * @param depthStencilMode 設定する深度ステンシルモード
	 */
	void SetMode(Mode depthStencilMode);

private:
	/// 現在の深度ステンシルモード
	Mode m_currentMode;

	/// 深度ステンシルモードごとの深度ステンシルステート
	ComPtr<ID3D11DepthStencilState> m_pStates[Mode::COUNT];

	/**
	 * @brief 全ての深度ステンシルステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState() final;
};