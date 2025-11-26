/******************************************************************//**
 * @file   SpriteDrawer.h
 * @brief  スプライトの描画を行う
 * 
 * @author 石田怜
 * @date   2025/11/25
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include "Material.h"

class SpriteDrawer
{
public:
	/**
	 * @brief 初期化を行う
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
	 * @brief スプライトの描画を行う
	 * @param material 描画に使用するマテリアル
	 */
	void Draw(const Material& material);

private:
	SpriteDrawer();

	/// スプライト描画用頂点情報
	struct SpriteVertex
	{
		DirectX::XMFLOAT2 pos;
		DirectX::XMFLOAT2 uv;
	};

	/// デバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// デバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// 頂点バッファ
	ComPtr<ID3D11Buffer> m_pVertexBuffer;

	/**
	 * @brief 頂点バッファを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateVertexBuffer();

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return SpriteDrawerへの参照
	 */
	static SpriteDrawer& Instance();
};
