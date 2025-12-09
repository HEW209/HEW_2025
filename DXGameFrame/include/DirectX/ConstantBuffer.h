/*****************************************************************//**
 * @file   ConstantBuffer.h
 * @brief  定数バッファを扱う
 * 
 * @author 石田怜
 * @date   2025/10/26
 *********************************************************************/
#pragma once

#include <DirectX/DirectXInclude.h>

/**
 * @brief 定数バッファを扱う
 */
class ConstantBuffer
{
public:
	/// ライト情報
	struct Light
	{
		DirectX::XMFLOAT3 lightDir;			// ライト方向
		float pad1;
		DirectX::XMFLOAT3 lightColor;		// ライト色
		float pad2;
		DirectX::XMFLOAT3 ambientColor;		// 環境光
		float pad3;
	};

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

	/**
	 * @brief ライト情報を設定する
	 * @param light ライト情報
	 */
	void SetLight(const Light& light);

private:
	ConstantBuffer();
	~ConstantBuffer() = default;

	/// WVP定数バッファのスロット番号
	enum class SlotNum : UINT
	{
		WVP,		// WVP行列
		LIGHT,		// ライト情報
		BONE,		// ボーン情報
		CUSTOM,		// カスタムパラメータ
		COUNT		// 使用スロット数
	};

	/// WVP行列
	struct WVP
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	/// ボーン情報
	struct Bone
	{

	};

	/// カスタムパラメータ
	struct CustomParameter
	{
		BYTE data[256];
	};

	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// WVP行列
	WVP m_WVP;

	/// WVP定数バッファ
	ComPtr<ID3D11Buffer> m_WVPBuffer;

	/// ライト情報
	Light m_light;

	/// ライト定数バッファ
	ComPtr<ID3D11Buffer> m_pLightBuffer;

	/**
	 * @brief WVP定数バッファを更新する
	 */
	void UpdateWVPBuffer();

	/**
	 * @brief WVP定数バッファを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateWVPBuffer();

	/**
	 * @brief ライト情報定数バッファを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateLightBuffer();

	/**
	 * @brief ボーン情報定数バッファを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateBoneBuffer();

	/**
	 * @brief カスタムパラメータ定数バッファを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateCustomBuffer();

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