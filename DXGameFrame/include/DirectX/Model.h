/******************************************************************//**
 * @file   Model.h
 * @brief  モデルデータを扱う
 * 
 * @author 石田怜
 * @date   2025/10/30
 *********************************************************************/
#pragma once

#include <Utility/Common.h>
#include "MeshGroup.h"
#include "Material.h"

/**
 * @brief モデルデータを扱う
 */
class Model
{
public:
	Model();
	~Model();

	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータへのファイルパス
	 * @return 読み込み結果
	 */
	bool Load(const std::string& filePath);

	/**
	 * @brief モデルの描画を行う
	 * @param materials 使用するマテリアル配列
	 */
	void Draw(const std::vector<Material>& materials);

	const std::vector<Material>& GetMaterials()
	{
		return m_materials;
	}

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
