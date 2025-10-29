/*****************************************************************//**
 * @file   BlendState.h
 * @brief  ブレンドステートを扱う
 * 
 * @author 石田怜
 * @date   2025/09/30
 *********************************************************************/
#pragma once

#include <DirectX/StateBase.h>

/**
 * @brief ブレンドステートを扱う
 */
class BlendState : public StateBase
{
public:
	BlendState();
	~BlendState() = default;

	/// ブレンドモード
	enum Mode
	{
		DEFAULT,		//ブレンド無し
		ALPHA,			//通常のアルファブレンド
		ADD,			//加算
		SUB,			//減算
		MUL,			//乗算
		COUNT			//ブレンドモードの種類数
	};

	/**
	 * @brief 終了処理
	 */
	void Uninit() final;

	/**
	 * @brief ブレンドモードを設定する
	 * @param blendMode 設定するブレンドモード
	 */
	void SetMode(Mode blendMode);

private:
	/// 現在のブレンドモード
	Mode m_currentMode;

	/// ブレンドモードごとのブレンドステート
	ComPtr<ID3D11BlendState> m_pStates[Mode::COUNT];

	/**
	 * @brief 全てのブレンドステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState() final;
};