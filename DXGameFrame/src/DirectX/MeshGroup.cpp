// MeshGroup.cpp
#include <DirectX/MeshGroup.h>
#include <functional>
#include <DirectX/ConstantBufferManager.h>

DirectX::XMMATRIX GetMatrixFromAssimpMatrix(aiMatrix4x4 M);

MeshGroup::MeshGroup() :
	m_playNo(ANIME_NONE),
	m_blendNo(ANIME_NONE),
	m_blendTime(0.0f),
	m_blendTotalTime(0.0f),
	m_parametricBlend(0.0f)
{
}

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
	CreateBones(pScene);
	CreateMesh(pScene);
}

HRESULT MeshGroup::Create(const Mesh::Description& desc)
{
	// メッシュの初期化
	m_meshes.clear();
	m_bones.clear();
	m_meshes.emplace_back();

	return m_meshes[0].Create(desc);
}

MeshGroup::AnimeNo MeshGroup::AddAnimation(const aiScene* pScene)
{
	// アニメーションチェック
	if (!pScene->HasAnimations())
		return ANIME_NONE;

	// アニメーションデータ確保
	aiAnimation* assimpAnime = pScene->mAnimations[0];
	m_animes.push_back(Animation());
	Animation& anime = m_animes.back();

	// アニメーション設定
	using XMVectorKey = std::pair<float, DirectX::XMVECTOR>;
	using XMVectorKeys = std::map<float, DirectX::XMVECTOR>;
	float fbxToGameFrame = static_cast<float>(assimpAnime->mTicksPerSecond);
	anime.info.totalTime = static_cast<float>(assimpAnime->mDuration) / fbxToGameFrame;
	anime.info.speed = 1.0f;

	for (UINT channelIdx = 0; channelIdx < assimpAnime->mNumChannels; ++channelIdx)
	{
		// これから処理するチャンネルの情報を取得
		aiNodeAnim* assimpChannel = assimpAnime->mChannels[channelIdx];

		// Assimp側の名称からノード名のみ抽出
		std::string name = assimpChannel->mNodeName.data;

		// 対応するチャンネル(ボーン)を既存のノード内から探索
		BoneIndex nodeIdx = FindNode(name.c_str());
		if (nodeIdx == BONE_NONE) { continue; }

		// 読み取り済みのノードか探索
		AnimeChannels::iterator channelIt =
			std::find_if(anime.channels.begin(), anime.channels.end(),
				[nodeIdx](const AnimeChannel& val) {
					return val.bone == nodeIdx;
				});

		// 始めて読み取るノードであれば新規追加
		if (channelIt == anime.channels.end())
		{
			anime.channels.push_back({});
			channelIt = anime.channels.end() - 1;
			channelIt->bone = nodeIdx;
		}

		// 各キーの値を配列にコピー
		XMVectorKeys keys[3];
		AnimeTimeline& timeline = channelIt->timeline;
		// 位置
		for (UINT i = 0; i < assimpChannel->mNumPositionKeys; ++i)
		{
			aiVectorKey& key = assimpChannel->mPositionKeys[i];
			keys[0].insert(XMVectorKey(static_cast<float>(key.mTime) / fbxToGameFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, 0.0f)
			));
		}
		// 回転
		for (UINT i = 0; i < assimpChannel->mNumRotationKeys; ++i)
		{
			aiQuatKey& key = assimpChannel->mRotationKeys[i];
			keys[1].insert(XMVectorKey(static_cast<float>(key.mTime) / fbxToGameFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w)));
		}
		// 拡縮
		for (UINT i = 0; i < assimpChannel->mNumScalingKeys; ++i)
		{
			aiVectorKey& key = assimpChannel->mScalingKeys[i];
			keys[2].insert(XMVectorKey(static_cast<float>(key.mTime) / fbxToGameFrame,
				DirectX::XMVectorSet(key.mValue.x, key.mValue.y, key.mValue.z, 0.0f)));
		}

		// 各タイムラインの先頭の参照を設定
		XMVectorKeys::iterator it[] = { keys[0].begin(), keys[1].begin(), keys[2].begin() };
		for (int i = 0; i < 3; ++i)
		{
			// キーが一つしかない場合は、参照終了
			if (keys[i].size() == 1)
				++it[i];
		}

		do
		{
			// 現状の参照位置で一番小さい時間を取得
			float time = anime.info.totalTime;
			for (int i = 0; i < 3; ++i)
			{
				if (it[i] != keys[i].end())
				{
					time = std::min(it[i]->first, time);
				}
			}

			// 時間に基づいて補間値を計算
			DirectX::XMVECTOR result[3];
			for (int i = 0; i < 3; ++i)
			{
				// 先頭のキーより小さい時間であれば、先頭の値を設定
				if (time < keys[i].begin()->first)
				{
					result[i] = keys[i].begin()->second;
				}
				// 最終キーより大きい時間であれば、最終の値を設定
				else if (keys[i].rbegin()->first <= time)
				{
					result[i] = keys[i].rbegin()->second;
					it[i] = keys[i].end();
				}
				// キー同士に挟まれた時間であれば、補間値を計算
				else
				{
					// 参照している時間と同じであれば、次の参照へキーを進める
					if (it[i]->first <= time)
					{
						++it[i];
					}

					// 補間値の計算
					XMVectorKeys::iterator prev = it[i];
					--prev;
					float rate = (time - prev->first) / (it[i]->first - prev->first);
					result[i] = DirectX::XMVectorLerp(prev->second, it[i]->second, rate);
				}
			}


			// キーが追加済みか判定
			AnimeTimeline::iterator timelineIt = timeline.find(time);
			if (timelineIt != timeline.end())
			{
				// キーの数に応じて値を更新
				if (assimpChannel->mNumPositionKeys > 1)
					DirectX::XMStoreFloat3(&timelineIt->second.translate, result[0]);
				if (assimpChannel->mNumRotationKeys > 1)
					DirectX::XMStoreFloat4(&timelineIt->second.quaternion, result[1]);
				if (assimpChannel->mNumScalingKeys > 1)
					DirectX::XMStoreFloat3(&timelineIt->second.scale, result[2]);
			}
			else
			{
				// 新しい値の追加
				AnimeTransform transform;
				DirectX::XMStoreFloat3(&transform.translate, result[0]);
				DirectX::XMStoreFloat4(&transform.quaternion, result[1]);
				DirectX::XMStoreFloat3(&transform.scale, result[2]);
				timeline.insert(AnimeKey(time, transform));
			}
		} while (it[0] != keys[0].end() || it[1] != keys[1].end() || it[2] != keys[2].end());
	}

	// アニメ番号を返す
	return static_cast<AnimeNo>(m_animes.size() - 1);
}

void MeshGroup::PlayAnime(AnimeNo no, bool loop, float speed)
{
	// 再生チェック
	if (!CheckAnimeNo(no)) { return; }
	if (m_playNo == no) { return; }

	// 合成アニメーションかチェック
	if (no != PARAMETRIC_ANIME)
	{
		// 通常の初期化
		InitAnime(no);
		m_animes[no].info.isLoop = loop;
		m_animes[no].info.speed = speed;
	}
	else
	{
		// 合成アニメーションの元になっているアニメーションを初期化
		InitAnime(m_parametric[0]);
		InitAnime(m_parametric[1]);
		m_animes[m_parametric[0]].info.isLoop = loop;
		m_animes[m_parametric[1]].info.isLoop = loop;
		SetParametricBlend(0.0f);
	}

	// 再生アニメーションの設定
	m_playNo = no;
}

void MeshGroup::PlayBlend(AnimeNo no, float blendTime, bool loop, float speed)
{
	// 再生チェック
	if (!CheckAnimeNo(no)) { return; }

	// 合成アニメーションかチェック
	if (no != PARAMETRIC_ANIME)
	{
		InitAnime(no);
		m_animes[no].info.isLoop = loop;
		m_animes[no].info.speed = speed;
	}
	else
	{
		// 合成アニメーションの元になっているアニメーションを初期化
		InitAnime(m_parametric[0]);
		InitAnime(m_parametric[1]);
		m_animes[m_parametric[0]].info.isLoop = loop;
		m_animes[m_parametric[1]].info.isLoop = loop;
		SetParametricBlend(0.0f);
	}

	// ブレンドの設定
	m_blendTime = 0.0f;
	m_blendTotalTime = blendTime;
	m_blendNo = no;
}

void MeshGroup::SetParametric(AnimeNo no1, AnimeNo no2)
{
	// アニメーションチェック
	if (!CheckAnimeNo(no1)) { return; }
	if (!CheckAnimeNo(no2)) { return; }

	// 合成設定
	m_parametric[0] = no1;
	m_parametric[1] = no2;
	SetParametricBlend(0.0f);
}

void MeshGroup::SetParametricBlend(float blendRate)
{
	// 合成元アニメが設定されているか確認
	if (m_parametric[0] == ANIME_NONE || m_parametric[1] == ANIME_NONE) return;

	// 合成割合設定
	m_parametricBlend = blendRate;

	// 割合に基づいてアニメーションの再生速度を設定
	Animation& anime1 = m_animes[m_parametric[0]];
	Animation& anime2 = m_animes[m_parametric[1]];
	float blendTotalTime =
		anime1.info.totalTime * (1.0f - m_parametricBlend) +
		anime2.info.totalTime * m_parametricBlend;
	anime1.info.speed = anime1.info.totalTime / blendTotalTime;
	anime2.info.speed = anime2.info.totalTime / blendTotalTime;
}

void MeshGroup::SetAnimeTime(AnimeNo no, float time)
{
	// アニメーションチェック
	if (!CheckAnimeNo(no)) { return; }
	if (no == PARAMETRIC_ANIME) { return; }

	// 再生時間変更
	Animation& anime = m_animes[no];
	anime.info.nowTime = time;
	CheckAnimePlayLoop(anime.info);
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

		// ボーンを設定
		DirectX::XMFLOAT4X4 bones[200];
		const Mesh::Description& desc = m_meshes[i].GetDesc();

		if (!m_bones.empty())
		{
			for (int boneIdx = 0; boneIdx < desc.bones.size(); ++boneIdx)
			{
				const Mesh::Bone& bone = desc.bones[boneIdx];
				DirectX::XMStoreFloat4x4(
					&bones[boneIdx],
					DirectX::XMMatrixTranspose(bone.invOffset * GetBoneMatrix(bone.nodeIndex))
				);
			}
			ConstantBufferManager::Instance().SetBone(bones);
		}

		// メッシュ描画
		m_meshes[i].Draw();
	}
}

int MeshGroup::GetMeshNum()
{
	return m_meshes.size();
}

Mesh* MeshGroup::GetMesh(int index)
{
	return &m_meshes[index];
}

DirectX::XMMATRIX MeshGroup::GetBoneMatrix(BoneIndex index)
{
	if (index < m_bones.size())
		return m_bones[index].matrix;
	return DirectX::XMMatrixIdentity();
}

void MeshGroup::CreateMesh(const aiScene* pScene)
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
		MakeVertexWeight(pScene, i, desc);

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

void MeshGroup::CreateBones(const aiScene* pScene)
{
	// 再帰処理でAssimpのノード情報を読み取り
	using FuncRecurciveMakeNodes = std::function<BoneIndex(aiNode*, BoneIndex, DirectX::XMMATRIX)>;
	FuncRecurciveMakeNodes func = [&func, this](aiNode* assimpNode, BoneIndex parent, DirectX::XMMATRIX mat)
	{
		// assimp側で階層情報が細かく分割されている場合の処理
		std::string name = assimpNode->mName.data;
		if (name.find("$AssimpFbx") != std::string::npos)
		{
			// 分割前のノードに到達するまで、各行列を掛け合わせていく
			DirectX::XMMATRIX transform = GetMatrixFromAssimpMatrix(assimpNode->mTransformation);
			mat = transform * mat;
			return func(assimpNode->mChildren[0], parent, mat);
		}

		// Assimpのノード情報を格納
		Bone bone;
		bone.name = name;
		bone.parentID = parent;
		bone.childrenIDs.resize(assimpNode->mNumChildren);
		bone.matrix = mat;

		// ノードの一覧に追加
		m_bones.push_back(bone);
		BoneIndex nodeIndex = static_cast<BoneIndex>(m_bones.size() - 1);

		// 子要素の情報を追加
		for (UINT i = 0; i < assimpNode->mNumChildren; ++i)
		{
			m_bones[nodeIndex].childrenIDs[i] =
				func(assimpNode->mChildren[i], nodeIndex, DirectX::XMMatrixIdentity());
		}

		return nodeIndex;
	};

	// ノード作成
	func(pScene->mRootNode, BONE_NONE, DirectX::XMMatrixIdentity());

	// アニメーション計算領域に、ノード数分の初期データを作成
	AnimeTransform init = {
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)
	};
	for (int i = 0; i < MAX_ANIMEPATTERN; ++i)
	{
		m_animeTransform[i].resize(m_bones.size(), init);
	}
}

MeshGroup::BoneIndex MeshGroup::FindNode(const char* name)
{
	// 構築済みのボーンノードから該当ノードを取得
	std::vector<Bone>::iterator it = std::find_if(m_bones.begin(), m_bones.end(),
		[name](const Bone& val) {
			return val.name == name;
		});
	if (it == m_bones.end()) {
		return BONE_NONE;
	}

	return static_cast<BoneIndex>(it - m_bones.begin());
}

void MeshGroup::MakeVertexWeight(const aiScene* pScene, int meshIndex, Mesh::Description& desc)
{
	aiMesh* assimpMesh = pScene->mMeshes[meshIndex];
	if (assimpMesh->HasBones())
	{
		MakeVertexWeightHasBone(assimpMesh, desc);
	}
	else
	{
		MakeVertexWeightFromNode(pScene, assimpMesh, desc);
	}
}

void MeshGroup::MakeVertexWeightHasBone(const aiMesh* pMesh, Mesh::Description& desc)
{
	// 読み取り用のウェイトデータ作成
	struct WeightPair
	{
		unsigned int idx;
		float weight;
	};
	std::vector<std::vector<WeightPair>> weights;
	weights.resize(desc.vtx.size());

	// メッシュに割り当てられているボーン領域確保
	desc.bones.resize(pMesh->mNumBones);
	for (auto boneIt = desc.bones.begin(); boneIt != desc.bones.end(); ++boneIt)
	{
		UINT boneIdx = static_cast<UINT>(boneIt - desc.bones.begin());
		aiBone* assimpBone = pMesh->mBones[boneIdx];

		// 構築済みのノードから該当ノードのインデックスを取得
		boneIt->nodeIndex = FindNode(assimpBone->mName.data);
		if (boneIt->nodeIndex == BONE_NONE)
		{
			continue;
		}

		// メッシュに割り当てられているボーンを、原点に戻す逆行列を設定
		boneIt->invOffset = GetMatrixFromAssimpMatrix(assimpBone->mOffsetMatrix);

		// ウェイトの設定
		for (UINT i = 0; i < assimpBone->mNumWeights; ++i)
		{
			aiVertexWeight& weight = assimpBone->mWeights[i];
			weights[weight.mVertexId].push_back({ boneIdx, weight.mWeight });
		}
	}

	// 取得してきた頂点ウェイトを頂点データに書き込み
	for (int i = 0; i < weights.size(); ++i)
	{
		const int maxWeight = 4;
		// プログラム側の割り当て数を超える場合の正規化処理
		if (weights[i].size() >= maxWeight)
		{
			// 割合が高い順にソート
			std::sort(weights[i].begin(), weights[i].end(),
				[](WeightPair& a, WeightPair& b) {
					return a.weight > b.weight;
				});
			// ウェイト数に合わせて正規化
			float total = 0.0f;
			for (int j = 0; j < maxWeight; ++j)
				total += weights[i][j].weight;
			for (int j = 0; j < maxWeight; ++j)
				weights[i][j].weight /= total;
		}

		// 正規化したデータを頂点データに割り当て
		for (int j = 0; j < weights[i].size() && j < maxWeight; ++j)
		{
			desc.vtx[i].index[j] = weights[i][j].idx;
			desc.vtx[i].weight[j] = weights[i][j].weight;
		}
	}
}

void MeshGroup::MakeVertexWeightFromNode(const aiScene* pScene, const aiMesh* pMesh, Mesh::Description& desc)
{
	// 階層内のメッシュを探索
	BoneIndex nodeIndex = FindNode(pMesh->mName.data);
	if (nodeIndex == BONE_NONE) { return; }

	// メッシュでない親ノードを再帰探索
	using FuncRecurciveFindNoMesh = std::function<int(int)>;
	FuncRecurciveFindNoMesh func = [&func, this, pScene](BoneIndex parent)
		{
			if (parent == BONE_NONE) { return BONE_NONE; }

			// 現在探索中のノード名がメッシュの一覧と一致するか確認
			std::string name = m_bones[parent].name;
			for (UINT i = 0; i < pScene->mNumMeshes; ++i)
			{
				if (name == pScene->mMeshes[i]->mName.data)
				{
					return func(m_bones[parent].parentID);
				}
			}

			// 一致しないノードを変換元として扱う
			return parent;
		};

	// ボーン割り当て情報を作成
	Mesh::Bone bone = {};
	bone.nodeIndex = func(m_bones[nodeIndex].parentID);
	if (bone.nodeIndex != BONE_NONE) {
		// ノードに保存されている行列を逆行列として設定
		bone.invOffset = DirectX::XMMatrixInverse(nullptr, m_bones[bone.nodeIndex].matrix);
		// ボーンの情報を設定
		desc.bones.resize(1);
		desc.bones[0] = bone;

		// 頂点すべてを割り当てたボーンに100%依存とする
		for (auto vtxIt = desc.vtx.begin(); vtxIt != desc.vtx.end(); ++vtxIt)
			vtxIt->weight[0] = 1.0f;
	}
}

void MeshGroup::StepAnime(float tick)
{
	// アニメーションの再生確認
	if (m_playNo == ANIME_NONE) { return; }

	//--- アニメーション行列の更新
	// パラメトリック
	if (m_playNo == PARAMETRIC_ANIME || m_blendNo == PARAMETRIC_ANIME)
	{
		CalcAnime(PARAMETRIC0, m_parametric[0]);
		CalcAnime(PARAMETRIC1, m_parametric[1]);
	}
	// メインアニメ
	if (m_playNo != ANIME_NONE && m_playNo != PARAMETRIC_ANIME)
	{
		CalcAnime(MAIN, m_playNo);
	}
	// ブレンドアニメ
	if (m_blendNo != ANIME_NONE && m_blendNo != PARAMETRIC_ANIME)
	{
		CalcAnime(BLEND, m_blendNo);
	}

	// アニメーション行列に基づいて骨行列を更新
	CalcBones(0, DirectX::XMMatrixIdentity());

	//--- アニメーションの時間更新
	// メインアニメ
	UpdateAnime(m_playNo, tick);
	// ブレンドアニメ
	if (m_blendNo != ANIME_NONE)
	{
		UpdateAnime(m_blendNo, tick);
		m_blendTime += tick;
		if (m_blendTime <= m_blendTime)
		{
			// ブレンドアニメの自動終了
			m_blendTime = 0.0f;
			m_blendTotalTime = 0.0f;
			m_playNo = m_blendNo;
			m_blendNo = ANIME_NONE;
		}
	}
	// パラメトリック
	if (m_playNo == PARAMETRIC_ANIME || m_blendNo == PARAMETRIC_ANIME)
	{
		UpdateAnime(m_parametric[0], tick);
		UpdateAnime(m_parametric[1], tick);
	}
}

void MeshGroup::InitAnime(AnimeNo no)
{
	// アニメの設定なし、パラメトリックで設定されているなら初期化しない
	if (no == ANIME_NONE || no == PARAMETRIC_ANIME) { return; }

	Animation& anime = m_animes[no];
	anime.info.nowTime = 0.0f;
	anime.info.speed = 1.0f;
	anime.info.isLoop = false;
}

void MeshGroup::UpdateAnime(AnimeNo no, float tick)
{
	// アニメーションチェック
	if (!CheckAnimeNo(no)) { return; }
	if (no == PARAMETRIC_ANIME) { return; }

	// アニメーション時間の更新
	Animation& anime = m_animes[no];
	SetAnimeTime(no, anime.info.nowTime + anime.info.speed * tick);
}

void MeshGroup::CalcAnime(AnimePattern kind, AnimeNo no)
{
	// アニメーションチェック
	if (!CheckAnimeNo(no)) { return; }
	if (no == PARAMETRIC_ANIME) { return; }

	Animation& anime = m_animes[no];
	AnimeChannels::iterator channelIt = anime.channels.begin();
	for (auto channelIt = anime.channels.begin(); channelIt != anime.channels.end(); ++channelIt)
	{
		// アニメーションで変更するボーンがなければスキップ
		if (channelIt->bone == BONE_NONE) { continue; }

		// タイムラインの設定がなければスキップ
		AnimeTimeline& timeline = channelIt->timeline;
		if (timeline.empty()) { continue; }

		// ボーンの姿勢をタイムラインに沿って更新
		AnimeTransform& transform = m_animeTransform[kind][channelIt->bone];
		if (timeline.size() <= 1)
		{
			// キーが一つしかないので値をそのまま使用
			transform = channelIt->timeline[0];
		}
		else
		{
			AnimeTimeline::iterator startIt = timeline.begin();
			if (anime.info.nowTime <= startIt->first)
			{
				// 先頭キーよりも前の時間なら、先頭の値を使用
				transform = startIt->second;
			}
			else if (timeline.rbegin()->first <= anime.info.nowTime)
			{
				// 最終キーよりも後の時間なら、最後の値を使用
				transform = timeline.rbegin()->second;
			}
			else
			{
				// 指定された時間を挟む2つのキーから、補間された値を計算
				AnimeTimeline::iterator nextIt = timeline.upper_bound(anime.info.nowTime);
				startIt = nextIt;
				--startIt;
				float rate = (anime.info.nowTime - startIt->first) / (nextIt->first - startIt->first);
				LerpTransform(&transform, startIt->second, nextIt->second, rate);
			}
		}
	}
}

void MeshGroup::CalcBones(BoneIndex index, const DirectX::XMMATRIX parent)
{
	AnimeTransform transform; // 合成結果の格納先

	// パラメトリックの合成
	if (m_playNo == PARAMETRIC_ANIME || m_blendNo == PARAMETRIC_ANIME)
	{
		LerpTransform(&transform,
			m_animeTransform[PARAMETRIC0][index],
			m_animeTransform[PARAMETRIC1][index],
			m_parametricBlend);

		// のちのブレンドとの合成のために、パラメトリックの結果を再生・ブレンドのいずれかに格納
		if (m_playNo == PARAMETRIC_ANIME) { m_animeTransform[MAIN][index] = transform; }
		if (m_blendNo == PARAMETRIC_ANIME) { m_animeTransform[BLEND][index] = transform; }
	}

	// ブレンドアニメの合成
	if (m_blendNo != ANIME_NONE)
	{
		LerpTransform(&transform,
			m_animeTransform[MAIN][index],
			m_animeTransform[BLEND][index],
			m_blendTime / m_blendTotalTime);
	}
	else
	{
		// 合成の必要がなかったので、メインの情報をそのまま格納
		transform = m_animeTransform[MAIN][index];
	}

	// 該当ノードの姿勢行列を計算
	Bone& node = m_bones[index];
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&transform.translate));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&transform.quaternion));
	DirectX::XMMATRIX S = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&transform.scale));
	node.matrix = (S * R * T) * parent;

	// 子要素の姿勢を更新
	std::vector<BoneIndex>::iterator it = node.childrenIDs.begin();
	while (it != node.childrenIDs.end())
	{
		CalcBones(*it, node.matrix);
		++it;
	}
}

void MeshGroup::LerpTransform(AnimeTransform* pOut, const AnimeTransform& a, const AnimeTransform& b, float rate)
{
	// 補間元の情報をベクトルに格納
	DirectX::XMVECTOR vec[][2] = {
		{ DirectX::XMLoadFloat3(&a.translate),	DirectX::XMLoadFloat3(&b.translate) },
		{ DirectX::XMLoadFloat4(&a.quaternion),	DirectX::XMLoadFloat4(&b.quaternion) },
		{ DirectX::XMLoadFloat3(&a.scale),		DirectX::XMLoadFloat3(&b.scale) },
	};

	// 補間(※Quaternionだけは球面補間が必要？
	for (int i = 0; i < 3; ++i)
		vec[i][0] = DirectX::XMVectorLerp(vec[i][0], vec[i][1], rate);

	// 計算結果の格納
	DirectX::XMStoreFloat3(&pOut->translate, vec[0][0]);
	DirectX::XMStoreFloat4(&pOut->quaternion, vec[1][0]);
	DirectX::XMStoreFloat3(&pOut->scale, vec[2][0]);
}

void MeshGroup::CheckAnimePlayLoop(AnimePlayInfo& info)
{
	if (info.isLoop) {
		while (info.nowTime >= info.totalTime)
		{
			info.nowTime -= info.totalTime;
		}
	}
}

bool MeshGroup::CheckAnimeNo(AnimeNo no)
{
	// パラメトリックアニメーション確認
	if (no == PARAMETRIC_ANIME)
	{
		// パラメトリック用のデータが両方正しく設定されているか
		return
			m_parametric[0] != ANIME_NONE &&
			m_parametric[1] != ANIME_NONE;
	}

	// 問題ないアニメーション番号かどうか
	return 0 <= no && no < m_animes.size();
}

DirectX::XMMATRIX GetMatrixFromAssimpMatrix(aiMatrix4x4 M)
{
	return DirectX::XMMatrixSet(
		M.a1, M.b1, M.c1, M.d1,
		M.a2, M.b2, M.c2, M.d2,
		M.a3, M.b3, M.c3, M.d3,
		M.a4, M.b4, M.c4, M.d4
	);
}
