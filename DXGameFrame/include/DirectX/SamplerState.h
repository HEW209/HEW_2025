/*****************************************************************//**
 * @file   SamplerState.h
 * @brief  サンプラーステートを扱う
 *
 * @author 石田怜
 * @date   2025/09/30
 *********************************************************************/
#pragma once

#include <DirectX/StateBase.h>

/**
 * @brief サンプラーステートを扱う
 */
class SamplerState : public StateBase
{
public:
	SamplerState();
	~SamplerState() = default;

	/// サンプラーモード
	enum Mode
	{
		DEFAULT = 0,			//LINEAR_WRAPが使用される
		LINEAR_WRAP = 0,		//線形補間 + 繰り返し
		LINEAR_CLAMP,			//線形補間 + 端固定
		POINT_WRAP,				//最近傍 + 繰り返し（ドット絵向け）
		POINT_CLAMP,			//最近傍 + 端固定（UI向け）
		ANISOTROPIC_WRAP,		//異方性 + 繰り返し
		ANISOTROPIC_CLAMP,		//異方性 + 端固定
		BORDER_BLACK,			//枠外黒
		COUNT
	};

	/**
	 * @brief 終了処理
	 */
	void Uninit() final;

	/**
	 * @brief サンプラーモードを設定する
	 * @param samplerMode 設定するサンプラーモード
	 * @param slotNum スロット番号
	 */
	void SetMode(Mode samplerMode, UINT slotNum = 0);

private:
	/// 現在のサンプラーモード
	Mode m_currentMode;

	/// サンプラーモードごとのサンプラーステート
	ComPtr<ID3D11SamplerState> m_pStates[Mode::COUNT];

	/**
	 * @brief 全てのサンプラーステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState() final;
};