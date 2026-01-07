// Model.cpp
#include <DirectX/Model.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <DirectX/Debug.h>

#if _MSC_VER >= 1930
#ifdef _DEBUG
#pragma comment(lib, "assimp/lib/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp/lib/assimp-vc143-mt.lib")
#endif
#elif _MSC_VER >= 1920
#ifdef _DEBUG
#pragma comment(lib, "assimp/lib/assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp/lib/assimp-vc142-mt.lib")
#endif
#elif _MSC_VER >= 1910
#ifdef _DEBUG
#pragma comment(lib, "assimp/lib/assimp-vc141-mtd.lib")
#else
#pragma comment(lib, "assimp/lib/assimp-vc141-mt.lib")
#endif
#endif

bool Model::Load(const std::string& filePath, ModelLoadType loadType)
{
	// モデル読み込み設定
	Assimp::Importer importer;
	UINT flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	flag |= aiProcess_MakeLeftHanded;

	// モデルデータ読み込み
	const aiScene* pScene = importer.ReadFile(filePath, flag);
	if (pScene == nullptr)
	{
#ifdef _DEBUG
		Debug::ErrorMessage(importer.GetErrorString());
#endif
		return false;
	}

	// ディレクトリの読み取り
	std::string directory = filePath;
	auto strIt = directory.begin();
	while (strIt != directory.end()) {
		if (*strIt == '/')
			*strIt = '\\';
		++strIt;
	}
	directory = directory.substr(0, directory.find_last_of('\\') + 1);

	// メッシュ作成
	m_meshGroup.Create(pScene, loadType);

	// マテリアル作成
	CreateMaterials(pScene, directory);

	// 読み込みタイプ毎の処理
	switch (loadType) {
	case ModelLoadType::OUTLINE:
		break;

	default:
		break;
	}

	return true;
}

bool Model::LoadAnimation(const std::string& filePath)
{
	// モデル読み込み設定
	Assimp::Importer importer;
	UINT flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	flag |= aiProcess_MakeLeftHanded;

	// モデルデータ読み込み
	const aiScene* pScene = importer.ReadFile(filePath, flag);
	if (pScene == nullptr)
	{
#ifdef _DEBUG
		Debug::ErrorMessage(importer.GetErrorString());
#endif
		return false;
	}

	auto it = m_animeNoMap.find(filePath);
	if (it != m_animeNoMap.end())
	{
		return it->second;
	}

	MeshGroup::AnimeNo animeNo;
	animeNo = m_meshGroup.AddAnimation(pScene);
	if (animeNo != MeshGroup::ANIME_NONE)
	{
		m_animeNoMap[filePath] = animeNo;
	}
	return animeNo;
}

void Model::PlayAnime(MeshGroup::AnimeNo no, bool loop, float speed)
{
	m_meshGroup.PlayAnime(no, loop, speed);
}

void Model::PlayBlend(MeshGroup::AnimeNo no, float blendTime, bool loop, float speed)
{
	m_meshGroup.PlayBlend(no, blendTime, loop, speed);
}

void Model::SetParametric(MeshGroup::AnimeNo no1, MeshGroup::AnimeNo no2)
{
	m_meshGroup.SetParametric(no1, no2);
}

void Model::SetParametricBlend(float blendRate)
{
	m_meshGroup.SetParametricBlend(blendRate);
}

void Model::SetAnimeTime(MeshGroup::AnimeNo no, float time)
{
	m_meshGroup.SetAnimeTime(no, time);
}

HRESULT Model::CreateMesh(const Mesh::Description& desc)
{
	m_materials.clear();
	m_materials.emplace_back();
	return m_meshGroup.Create(desc);
}

void Model::Draw(const std::vector<Material>& materials)
{
	m_meshGroup.Draw(materials);
}

void Model::StepAnime(float tick)
{
	m_meshGroup.StepAnime(tick);
}

const std::vector<Material>& Model::GetMaterials()
{
	return m_materials;
}

void Model::CreateMaterials(const aiScene* pScene, const std::string& directory)
{
	// マテリアル数取得
	UINT materialCount;
	materialCount = pScene->mNumMaterials;

	// マテリアル配列の初期化
	m_materials.clear();
	m_materials.resize(materialCount);

	// マテリアルパラメータの設定
	for (UINT i = 0; i < materialCount; ++i)
	{
		// テクスチャ読み込み処理
		aiString path;

		// テクスチャのパス情報を読み込み
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) != AI_SUCCESS)
			continue;

		// テクスチャの読み込み
		m_materials[i].SetTexture(directory + path.C_Str());
	}
}