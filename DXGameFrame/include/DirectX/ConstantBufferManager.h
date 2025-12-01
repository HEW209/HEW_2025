/******************************************************************//**
 * @file   ConstantBufferManager.h
 * @brief  定数バッファを管理する
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <vector>

/// 最大ボーン数
constexpr UINT MaxBone = 64;

/// カスタム定数バッファサイズ
constexpr UINT CustomCBSize = 256;

/// ライト定数バッファ
struct LightConstantBuffer
{
	DirectX::XMFLOAT3 lightDir;			// ライト方向
	float pad1;
	DirectX::XMFLOAT3 lightColor;		// ライト色
	float pad2;
	DirectX::XMFLOAT3 ambientColor;		// 環境光
	float pad3;
};

/// ボーン定数バッファ
struct BoneConstantBuffer
{
	DirectX::XMFLOAT4X4 bones[MaxBone];		// ボーン行列
};

/// カスタム定数バッファ
struct CustomConstantBuffer
{
	BYTE data[CustomCBSize];	// カスタムデータ
};

/**
 * @brief 定数バッファを扱う
 */
class ConstantBufferManager
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
	void SetView(const DirectX::XMFLOAT4X4& view, bool bufferUpdate = false);

	/**
	 * @brief プロジェクション行列を設定する
	 * @param view 設定するプロジェクション行列
	 * @param bufferUpdate バッファ更新フラグ
	 */
	void SetProjection(const DirectX::XMFLOAT4X4& projection, bool bufferUpdate = false);

	/**
	 * @brief ライト情報を設定する
	 * @param light ライト定数バッファ
	 */
	void SetLight(const LightConstantBuffer& light);

	/**
	 * @brief ボーン情報を設定する
	 * @param bones ボーン定数バッファ
	 */
	void SetBone(DirectX::XMFLOAT4X4 bones[MaxBone]);

	/**
	 * @brief カスタムデータを設定する
	 * @param data カスタムデータへのポインタ
	 */
	void SetCustomData(const std::vector<BYTE>& data);

private:
	ConstantBufferManager();
	~ConstantBufferManager() = default;

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
		DirectX::XMFLOAT4X4 world;		// ワールド行列
		DirectX::XMFLOAT4X4 view;		// ビュー行列
		DirectX::XMFLOAT4X4 projection;	// プロジェクション行列
	};

	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	/// WVP行列
	WVP m_WVP;

	/// WVP定数バッファ
	ComPtr<ID3D11Buffer> m_pWVPBuffer;

	/// ライト定数バッファ
	ComPtr<ID3D11Buffer> m_pLightBuffer;

	/// ボーン定数バッファ
	ComPtr<ID3D11Buffer> m_pBoneBuffer;

	/// カスタム定数バッファ
	ComPtr<ID3D11Buffer> m_pCustomBuffer;

	/**
	 * @brief WVP定数バッファを更新する
	 */
	void UpdateWVPBuffer();

	/**
	 * @brief 全ての定数バッファを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateAllBuffer();

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return ConstantBufferインスタンスへの参照
	 */
	static ConstantBufferManager& Instance();
};