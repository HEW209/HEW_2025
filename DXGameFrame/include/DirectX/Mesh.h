/******************************************************************//**
 * @file   Mesh.h
 * @brief  メッシュを扱う
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <vector>

/**
 * @brief メッシュを扱う
 */
class Mesh
{
public:
	Mesh();
	~Mesh() = default;

	/// 頂点情報
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;			// 座標
		DirectX::XMFLOAT3 normal;		// 法線
		DirectX::XMFLOAT2 uv;			// UV座標
		DirectX::XMFLOAT4 color;		// 頂点カラー
		float weight[4];				// ボーンへのウェイト
		UINT index[4];					// 対応するボーンへのインデックス
	};

	// ボーン情報
	struct Bone
	{
		int nodeIndex;	// 変形情報は階層情報に格納
		DirectX::XMMATRIX invOffset;	// 逆行列
	};
	using Bones = std::vector<Bone>;

	/// メッシュ情報
	struct Description
	{
		std::vector<Vertex> vtx;		// 頂点データ
		std::vector<UINT> idx;			// インデックスバッファのデータ
		UINT materialID;				// 対応マテリアルID
		bool isWrite;					// 動的な頂点の書き換え可能フラグ
		Bones bones;					// 対応ボーン
		D3D11_PRIMITIVE_TOPOLOGY topology;		// トポロジー設定
	};

	/**
	 * @brief メッシュ情報からメッシュを作成する
	 * @param desc メッシュ情報
	 * @return 成功したかを返す
	 */
	HRESULT Create(const Description& desc);

	/**
	 * @brief メッシュを描画する
	 */
	void Draw();

	/**
	 * @brief メッシュ情報を取得
	 * @return メッシュ情報
	 */
	const Description& GetDesc() const;

	/**
	 * @brief 対応マテリアルIDを取得
	 * @return このメッシュに使用するマテリアルのID
	 */
	UINT GetMaterialID() const;

	// 対応ボーン設定
	void SetBones(const Bones& bones);

private:
	/// メッシュ情報
	Description m_desc;

	/// 頂点バッファ
	ComPtr<ID3D11Buffer> m_pVtxBuffer;

	/// インデックスバッファ
	ComPtr<ID3D11Buffer> m_pIdxBuffer;

	/**
	 * @brief 頂点バッファを作成する
	 * @param vtx 頂点配列
	 * @param isWrite 書き込み可能フラグ
	 * @return 成功したかを返す
	 */
	HRESULT CreateVertexBuffer(const std::vector<Vertex>& vtx, bool isWrite = false);

	/**
	 * @brief インデックスバッファを作成する
	 * @param pIdx インデックス配列
	 * @return 成功したかを返す
	 */
	HRESULT CreateIndexBuffer(const std::vector<UINT>& idx);
};
