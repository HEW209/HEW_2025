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
	DirectX::XMFLOAT3 lightDir;				// ライト方向
	float lightIntensity;					// ライトの強さ
	DirectX::XMFLOAT3 lightColor;			// ライト色
	float pad1;
	DirectX::XMFLOAT3 ambientColor;			// 環境光
	float pad2;
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
	 */
	void SetWorld(const DirectX::XMMATRIX& world);

	/**
	 * @brief ビュー行列を設定する
	 * @param view 設定するビュー行列
	 */
	void SetView(const DirectX::XMMATRIX& view);

	/**
	 * @brief プロジェクション行列を設定する
	 * @param view 設定するプロジェクション行列
	 */
	void SetProjection(const DirectX::XMMATRIX& projection);

	/**
	 * @brief ライト定数バッファを設定する
	 * @param light 設定するライト定数バッファ情報
	 */
	void SetLight(const LightConstantBuffer& light);

	/**
	 * @brief 現在の経過時間を設定する
	 * @param time ゲーム開始から現在の経過時間
	 */
	void SetTime(float time);

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

	/**
	 * @brief フレーム更新定数バッファを更新する
	 */
	void UpdateFrameConstantBuffer();

private:
	ConstantBufferManager();
	~ConstantBufferManager() = default;

	/// WVP定数バッファのスロット番号
	enum class SlotNum : UINT
	{
		WORLD,		// ワールド行列
		FRAME,		// フレーム更新情報
		BONE,		// ボーン情報
		CUSTOM,		// カスタムパラメータ
		COUNT		// 使用スロット数
	};

	/// ワールド行列定数バッファ
	struct WorldConstantBuffer
	{
		DirectX::XMFLOAT4X4 world;		// ワールド行列
		DirectX::XMFLOAT4X4 invWorld;	// ワールド逆行列
	};

	/// フレーム更新定数バッファ
	struct FrameConstantBuffer
	{
		DirectX::XMFLOAT4X4 view;				// ビュー行列
		DirectX::XMFLOAT4X4 projection;			// プロジェクション行列


		DirectX::XMFLOAT3 cameraPos;			// カメラ位置
		float time;								// 経過時間

		LightConstantBuffer lightCB;			// ライト定数バッファ
	};

	/// D3Dデバイスへのポインタ
	ID3D11Device* m_pDevice;

	/// D3Dデバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_pContext;

	// フレーム更新定数バッファの情報
	FrameConstantBuffer m_frameCB;

	/// ワールド行列定数バッファ
	ComPtr<ID3D11Buffer> m_pWorldBuffer;

	/// フレーム更新定数バッファ
	ComPtr<ID3D11Buffer> m_pFrameBuffer;

	/// ボーン定数バッファ
	ComPtr<ID3D11Buffer> m_pBoneBuffer;

	/// カスタム定数バッファ
	ComPtr<ID3D11Buffer> m_pCustomBuffer;

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