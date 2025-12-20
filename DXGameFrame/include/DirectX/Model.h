/******************************************************************//**
 * @file   Model.h
 * @brief  モデルデータを扱う
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "DXTypes.h"
#include "MeshGroup.h"
#include "Material.h"

/**
 * @brief モデルデータを扱う
 */
class Model
{
public:
	Model() = default;
	~Model() = default;

	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータへのファイルパス
	 * @param loadType モデルの読み込みタイプ
	 * @return 実行結果
	 */
	bool Load(const std::string& filePath, ModelLoadType loadType);

	/**
	 * @brief 単一メッシュのモデルを作成する
	 * @param desc メッシュ情報
	 * @return 成功したかを返す
	 */
	HRESULT CreateMesh(const Mesh::Description& desc);

	/**
	 * @brief モデルの描画を行う
	 * @param materials 使用するマテリアル配列
	 */
	void Draw(const std::vector<Material>& materials);

	/**
	 * @brief マテリアル配列を取得する
	 * @return マテリアル配列への参照
	 */
	const std::vector<Material>& GetMaterials();

private:
	/// メッシュグループ
	MeshGroup m_meshGroup;

	/// マテリアル配列
	std::vector<Material> m_materials;

	/**
	 * @brief マテリアルを作成する
	 * @param pScene assimpシーンへのポインタ
	 * @param directory ディレクトリパス
	 */
	void CreateMaterials(const aiScene* pScene, const std::string& directory);
};
