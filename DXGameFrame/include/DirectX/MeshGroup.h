/******************************************************************//**
 * @file   MeshGroup.h
 * @brief  モデル単位のメッシュの組み合わせ
 * 
 * @author 石田怜
 * @date   2025/11/09
 *********************************************************************/
#pragma once

#include <DirectX/Mesh.h>
#include <DirectX/Material.h>
#include <assimp/scene.h>

/**
 * @brief モデル単位のメッシュの組み合わせ
 */
class MeshGroup
{
public:
	MeshGroup() = default;
	~MeshGroup() = default;

	/**
	 * @brief メッシュグループを作成する
	 * @param pScene assimpのシーンへのポインタ
	 */
	void Create(const aiScene* pScene);

	/**
	 * @brief メッシュグループの描画処理
	 * @param materials 使用するマテリアル配列
	 */
	void Draw(const std::vector<Material>& materials);

private:
	/// ボーン情報
	struct Bone
	{
		std::string name;							// ボーン名
		unsigned int parentID;						// 親ボーンID
		std::vector<unsigned int> childrenIDs;		// 子ボーンID配列
		DirectX::XMMATRIX matrix;					// 変換行列
	};

	/// メッシュ配列
	std::vector<Mesh> m_meshes;

	/// ボーン配列
	std::vector<Bone> m_bones;
};
