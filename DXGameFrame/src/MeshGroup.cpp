//MeshGroup.cpp
#include <DirectX/MeshGroup.h>

void MeshGroup::Create(const aiScene* pScene)
{
	// 事前準備
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	unsigned int meshCount = pScene->mNumMeshes;	// メッシュ数

	// メッシュ配列のサイズ設定
	m_meshes.resize(meshCount);

	// メッシュの作成
	for (unsigned int i = 0; i < meshCount; ++i)
	{
		Mesh::Description desc;		// メッシュ情報
		aiMesh* pMesh;				// assimpのメッシュ情報
		unsigned int vtxCount;		// 頂点数
		unsigned int faceCount;		// ポリゴン数

		// メッシュ読み込み準備
		pMesh = pScene->mMeshes[i];
		vtxCount = pMesh->mNumVertices;
		faceCount = pMesh->mNumFaces;

		// 頂点・インデックス配列のサイズ設定
		desc.vtx.resize(vtxCount);
		desc.idx.resize(faceCount * 3);		// 1ポリゴンで3インデックス

		// 頂点データの書き込み
		for (unsigned int j = 0; j < vtxCount; ++j)
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

		// ボーン生成
		// ここにボーン生成処理

		// インデックスの書き込み
		for (unsigned int j = 0; j < faceCount; ++j)
		{
			// モデルデータから値の取得
			aiFace face = pScene->mMeshes[i]->mFaces[j];

			// 値を設定 (右回転から左回転に)
			int idxOffset = j * 3;
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

void MeshGroup::Draw(const std::vector<Material>& materials)
{
	// メッシュグループの描画処理
	unsigned int meshCount = m_meshes.size();
	for (int i = 0; i < meshCount; ++i)
	{
		// マテリアルを設定
		unsigned int materialID = m_meshes[i].GetMaterialID();
		materials[materialID].Bind();

		// メッシュ描画
		m_meshes[i].Draw();
	}
}