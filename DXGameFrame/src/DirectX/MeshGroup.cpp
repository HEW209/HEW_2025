// MeshGroup.cpp
#include <DirectX/MeshGroup.h>

#include <map>

// 頂点比較用
struct Vec3Comparator {
	bool operator()(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) const {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
	}
};

// スムース法線を計算して頂点データに埋め込む関数
void CalcSmoothNormals(std::vector<Mesh::Vertex>& vertices)
{
	// 位置ごとの法線合計を蓄積するマップ
	// key: 頂点座標, value: 法線の合計ベクトル
	std::map<DirectX::XMFLOAT3, DirectX::XMVECTOR, Vec3Comparator> positionToNormalSum;

	// 全頂点を走査し、同じ位置にある頂点の法線を加算
	for (const auto& v : vertices)
	{
		DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&v.normal);

		if (positionToNormalSum.find(v.pos) == positionToNormalSum.end())
		{
			positionToNormalSum[v.pos] = normal;
		}
		else
		{
			positionToNormalSum[v.pos] = DirectX::XMVectorAdd(positionToNormalSum[v.pos], normal);
		}
	}

	// 加算された法線を平均化
	for (auto& pair : positionToNormalSum)
	{
		pair.second = DirectX::XMVector3Normalize(pair.second);
	}

	// スムース法線を各頂点に適用
	for (auto& v : vertices)
	{
		// 自分の位置に対応する平均化法線を取得
		DirectX::XMVECTOR smoothNorm = positionToNormalSum[v.pos];
		DirectX::XMStoreFloat4(&v.color, smoothNorm);
	}
}

void MeshGroup::Create(const aiScene* pScene, ModelLoadType loadType)
{
	// 事前準備
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	UINT meshCount = pScene->mNumMeshes;	// メッシュ数

	// メッシュ配列のサイズ設定
	m_meshes.resize(meshCount);

	// メッシュの作成
	for (UINT i = 0; i < meshCount; ++i)
	{
		Mesh::Description desc;		// メッシュ情報
		aiMesh* pMesh;				// assimpのメッシュ情報
		UINT vtxCount;				// 頂点数
		UINT faceCount;				// ポリゴン数

		// メッシュ読み込み準備
		pMesh = pScene->mMeshes[i];
		vtxCount = pMesh->mNumVertices;
		faceCount = pMesh->mNumFaces;

		// 頂点・インデックス配列のサイズ設定
		desc.vtx.resize(vtxCount);
		desc.idx.resize(faceCount * 3);		// 1ポリゴンで3インデックス

		// 頂点データの書き込み
		for (UINT j = 0; j < vtxCount; ++j)
		{
			// モデルデータから値の取得
			aiVector3D pos = pMesh->mVertices[j];
			aiVector3D normal = pMesh->HasNormals() ? pMesh->mNormals[j] : zero3;
			aiVector3D uv = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][j] : zero3;
			aiColor4D color = pMesh->HasVertexColors(0) ? pMesh->mColors[0][j] : one4;

			// 値を設定
			desc.vtx[j].pos = DirectX::XMFLOAT3(pos.x, pos.y, pos.z);
			desc.vtx[j].normal = DirectX::XMFLOAT3(normal.x, normal.y, normal.z);
			desc.vtx[j].uv = DirectX::XMFLOAT2(uv.x, uv.y);
			desc.vtx[j].color = DirectX::XMFLOAT4(color.r, color.g, color.b, color.a);
		}

		if (loadType == ModelLoadType::OUTLINE) {
			CalcSmoothNormals(desc.vtx);
		}

		// ボーン生成
		// ここにボーン生成処理

		// インデックスの書き込み
		for (UINT j = 0; j < faceCount; ++j)
		{
			// モデルデータから値の取得
			aiFace face = pScene->mMeshes[i]->mFaces[j];

			// 値を設定 (右回転から左回転に)
			UINT idxOffset = j * 3;
			desc.idx[idxOffset + 0] = face.mIndices[0];
			desc.idx[idxOffset + 1] = face.mIndices[2];
			desc.idx[idxOffset + 2] = face.mIndices[1];
		}

		// マテリアルの割り当て
		desc.materialID = pMesh->mMaterialIndex;

		// その他の設定
		desc.isWrite = false;
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// メッシュ作成
		m_meshes[i].Create(desc);
	}
}

HRESULT MeshGroup::Create(const Mesh::Description& desc)
{
	// メッシュの初期化
	m_meshes.clear();
	m_bones.clear();
	m_meshes.emplace_back();

	return m_meshes[0].Create(desc);
}

void MeshGroup::Draw(const std::vector<Material>& materials)
{
	// メッシュグループの描画処理
	UINT meshCount = (UINT)m_meshes.size();
	for (UINT i = 0; i < meshCount; ++i)
	{
		// マテリアルを設定
		UINT materialID = m_meshes[i].GetMaterialID();
		materials[materialID].Bind();

		// メッシュ描画
		m_meshes[i].Draw();
	}
}