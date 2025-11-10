/*****************************************************************//**
 * @file   MeshRenderer.h
 * @brief  メッシュの描画を行う
 * 
 * @author 石田怜
 * @date   2025/10/25
 *********************************************************************/
#pragma once

#include <Component/Renderer.h>
#include <DirectX/Material.h>
#include <DirectX/Model.h>
#include <memory>

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

	//void SetGeometry()

	/**
	 * @brief マテリアルを取得する
	 * @param slot マテリアルスロット番号
	 * @return マテリアルへのポインタ
	 */
	Material* GetMaterial(UINT slot);

	/**
	 * @brief マテリアルを設定する
	 * @param pMaterial マテリアルへのポインタ
	 * @param slot マテリアルスロット番号
	 */
	void SetMaterial(Material* pMaterial, UINT slot);

private:
	/// モデルへのポインタ
	std::shared_ptr<Model> m_pModel;

	/// マテリアル配列
	std::vector<Material> m_materials;
};
