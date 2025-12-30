/*****************************************************************//**
 * @file   MeshRenderer.h
 * @brief  メッシュの描画を行う
 *
 * @author 石田怜
 * @date   2025/11/24
 * 
 * @date   2025/12/28 [芝晃佑]	シャドウ描画対応
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
	virtual ~MeshRenderer();

	void Update() override;

	/**
	 * @brief 描画処理
	 */
	virtual void Draw() override;

	/**
	 * @brief 影描画処理
	 */
	virtual void DrawShadow() override;

	/**
	 * @brief 深度描画処理
	 */
	virtual void DrawDepth() override;

	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータへのファイルパス
	 */
	virtual void LoadModel(const std::string& filePath);

	/**
	 * @brief アニメーションデータを読み込む
	 * @param filePath アニメーションデータへのファイルパス
	 * @return アニメーション操作に使用するアニメ番号
	 */
	MeshGroup::AnimeNo LoadAnimation(const std::string& filePath);

	/*
	 * @brief アニメーションの再生
	 * @param no 再生するアニメーション番号
	 * @param loop ループ再生フラグ
	 * @param speed 再生速度
	 */
	void PlayAnime(MeshGroup::AnimeNo no, bool loop, float speed = 1.0f);

	/*
	 * @brief ブレンド再生
	 * @param no アニメーション番号
	 * @param blendTime ブレンドに掛ける時間
	 * @param loop ループフラグ
	 * @param speed 再生速度
	 */
	void PlayBlend(MeshGroup::AnimeNo no, float blendTime, bool loop, float speed = 1.0f);

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

	void SetShouldDrawShadow(bool shouldDrawShadow);

protected:
	/// モデルへのポインタ
	std::shared_ptr<Model> m_pModel;

	/// マテリアル配列
	std::vector<Material> m_materials;

	/// 影を描画するかどうかのフラグ
	bool m_shouldDrawShadow;
};
