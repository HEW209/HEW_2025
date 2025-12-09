/*****************************************************************//**
 * @file   MeshRenderer.h
 * @brief  メッシュの描画を行う
 *
 * @author 石田怜
 * @date   2025/11/24
 *********************************************************************/
#pragma once

#include <Component/Renderer.h>
#include <DirectX/Material.h>
#include <DirectX/Model.h>
#include <memory>

/**
 * @brief メッシュの描画を行う
 */
class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	~MeshRenderer() = default;

	/**
	 * @brief 描画処理
	 */
	void Draw() override;

	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータへのファイルパス
	 */
	void LoadModel(const std::string& filePath);

	/**
	 * @brief マテリアル数を取得する
	 * @return 現在のマテリアル数
	 */
	UINT GetMaterialNum();

	/**
	 * @brief マテリアルを取得する
	 * @param slot マテリアルスロット番号
	 * @return マテリアルへのポインタ
	 */
	Material* GetMaterial(UINT slot);

	/**
	 * @brief 全てのマテリアルを取得する
	 * @return マテリアル配列へポインタ
	 */
	std::vector<Material>* GetMaterials();

private:
	/// モデルへのポインタ
	std::shared_ptr<Model> m_pModel;

	/// マテリアル配列
	std::vector<Material> m_materials;
};
