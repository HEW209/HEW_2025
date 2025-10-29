/*****************************************************************//**
 * @file   ConstantBuffer.h
 * @brief  定数バッファを扱う
 * 
 * @author 石田怜
 * @date   2025/10/26
 *********************************************************************/
#pragma once

#include <DirectX/DirectXInclude.h>

/// WVP定数バッファのスロット番号
constexpr UINT WVPSlotNum = 0;

/**
 * @brief 定数バッファを扱う
 */
class ConstantBuffer
{
public:
	/**
	 * @brief 定数バッファの初期化
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
	 * @brief ワールド行列を設定する
	 * @param world 設定するワールド行列
	 * @param bufferUpdate バッファ更新フラグ
	 */
	void SetWorld(const DirectX::XMFLOAT4X4& world, bool bufferUpdate = true);

	/**
	 * @brief ビュー行列を設定する
	 * @param view 設定するビュー行列
	 * @param bufferUpdate バッファ更新フラグ
	 */
	void SetView(const DirectX::XMFLOAT4X4&, bool bufferUpdate = false);

	/**
	 * @brief プロジェクション行列を設定する
	 * @param view 設定するプロジェクション行列
	 * @param bufferUpdate バッファ更新フラグ
	 */
	void SetProjection(const DirectX::XMFLOAT4X4&, bool bufferUpdate = false);

private:
	ConstantBuffer();
	~ConstantBuffer() = default;

	/// WVP行列
	struct WVP
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// WVP行列
	WVP m_WVP;

	/// WVP定数バッファ
	ComPtr<ID3D11Buffer> m_WVPBuffer;

	/**
	 * @brief WVP定数バッファを更新する
	 */
	void UpdateWVPBuffer();

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return ConstantBufferインスタンスへの参照
	 */
	static ConstantBuffer& Instance()
	{
		static ConstantBuffer s_instance;
		return s_instance;
	}
};