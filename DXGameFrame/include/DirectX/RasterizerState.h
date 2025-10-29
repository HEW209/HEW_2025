/*****************************************************************//**
 * @file   RasterizerState.h
 * @brief  ラスタライザーステートを扱う
 *
 * @author 石田怜
 * @date   2025/10/08
 *********************************************************************/
#pragma once

#include <DirectX/StateBase.h>

/**
 * @brief ラスタライザーステートを扱う
 */
class RasterizerState : public StateBase
{
public:
	RasterizerState();
	~RasterizerState() = default;

	/// ラスタライズモード
	enum Mode
	{
		DEFAULT,        //デフォルト (前面描画)
		BACK,           //背面描画
		NONE,           //両面描画
		SCISSOR,		//シザー矩形を利用する(両面描画)
		WIREFRAME,      //ワイヤーフレーム描画
		COUNT
	};

	/**
	 * @brief 終了処理
	 */
	void Uninit() final;

	/**
	 * @brief ラスタライズモードを設定する
	 * @param rasterizeMode 設定するラスタライズモード
	 */
	void SetMode(Mode rasterizeMode);

private:
	/// 現在のラスタライズモード
	Mode m_currentMode;

	/// ラスタライズモードごとのラスタライザーステート
	ComPtr<ID3D11RasterizerState> m_pStates[Mode::COUNT];

	/**
	 * @brief 全てのラスタライザーステートを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllState() final;
};
