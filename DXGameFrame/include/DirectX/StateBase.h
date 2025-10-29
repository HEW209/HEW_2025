/*****************************************************************//**
 * @file   StateBase.h
 * @brief  DirectX描画ステートの基底クラス
 * 
 * @author 石田怜
 * @date   2025/10/23
 *********************************************************************/
#pragma once

#include <DirectX/DirectXInclude.h>

/**
 * @brief DirectX描画ステートの基底クラス
 */
class StateBase
{
public:
	StateBase();
	virtual ~StateBase() = default;

	/**
	 * @brief ステートの初期化
	 * @param pDevice D3Dデバイスへのポインタ
	 * @param pContext D3Dデバイスコンテキストへのポインタ
	 * @return 成功したかを返す
	 */
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/**
	 * @brief 終了処理
	 */
	virtual void Uninit() {}

protected:
	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

private:
	/**
	 * @brief 全てのステートを作成する
	 * @return 
	 */
	virtual HRESULT CreateAllState() = 0;
};