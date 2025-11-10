//Model.cpp
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

Model::Model()
{

}

Model::~Model()
{

}

bool Model::Load(const std::string& filePath)
{
	//モデル読み込み設定
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	flag |= aiProcess_MakeLeftHanded;

	//モデルデータ読み込み
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
	m_meshGroup.Create(pScene);

	// マテリアル作成
	CreateMaterials(pScene, directory);

	return true;
}

void Model::Draw(const std::vector<Material>& materials)
{
	m_meshGroup.Draw(materials);
}

void Model::CreateMaterials(const aiScene* pScene, const std::string& directory)
{
	// マテリアル数取得
	unsigned int materialCount;
	materialCount = pScene->mNumMaterials;

	// マテリアル配列の初期化
	m_materials.clear();
	m_materials.resize(materialCount);

	// マテリアルパラメータの設定
	for (unsigned int i = 0; i < materialCount; ++i)
	{
		// テクスチャ読み込み処理
		aiString path;

		// テクスチャのパス情報を読み込み
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) != AI_SUCCESS) {
			continue;
		}

		m_materials[i].SetTexture(directory + path.C_Str());
		continue;

		// モデルと同じ階層を探索
		// パスからファイル名のみ取得
		std::string fullPath = path.C_Str();
		std::string::iterator strIt = fullPath.begin();
		while (strIt != fullPath.end()) {
			if (*strIt == '/')
				*strIt = '\\';
			++strIt;
		}
		size_t find = fullPath.find_last_of("\\");
		std::string fileName = fullPath;
		if (find != std::string::npos)
			fileName = fileName.substr(find + 1);

		// テクスチャの読込

	}
}