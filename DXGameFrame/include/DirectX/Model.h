/******************************************************************//**
 * @file   Model.h
 * @brief  モデルデータを扱う
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "MeshGroup.h"
#include "Material.h"
#include <unordered_map>

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
	 * @return 実行結果
	 */
	bool Load(const std::string& filePath);

	/**
	 * @brief アニメーションデータを読み込む
	 * @param filePath アニメーションデータへのファイルパス
	 * @return 実行結果
	 */
	bool LoadAnimation(const std::string& filePath);

	/*
	 * @brief アニメーションの再生
	 * @param[in] no 再生するアニメーション番号
	 * @param[in] loop ループ再生フラグ
	 * @param[in] speed 再生速度
	 */
	void PlayAnime(MeshGroup::AnimeNo no, bool loop, float speed = 1.0f);

	/*
	 * @brief ブレンド再生
	 * @param[in] no アニメーション番号
	 * @param[in] blendTime ブレンドに掛ける時間
	 * @param[in] loop ループフラグ
	 * @param[in] speed 再生速度
	 */
	void PlayBlend(MeshGroup::AnimeNo no, float blendTime, bool loop, float speed = 1.0f);

	/*
	 * @brief アニメーションの合成再生
	 * @param[in] no1 合成元１
	 * @param[in] no2 合成元２
	 */
	void SetParametric(MeshGroup::AnimeNo no1, MeshGroup::AnimeNo no2);

	/*
	 * @brief 合成割合を指定
	 * @param[in] blendRate 0～1の間でno1とno2のアニメを合成(0でno1,1でno2)
	 */
	void SetParametricBlend(float blendRate);

	/*
	* @brief アニメーションの現在の再生時間を変更
	* @param[in] no 変更対象のアニメーション番号
	* @param[in] time 変更後の時間
	*/
	void SetAnimeTime(MeshGroup::AnimeNo no, float time);

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
	 * @brief アニメーション更新
	 * @param tick 経過フレーム
	 */
	void StepAnime(float tick);

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

	/// アニメ番号のマップ
	std::unordered_map<std::string, MeshGroup::AnimeNo> m_animeNoMap;

	/**
	 * @brief マテリアルを作成する
	 * @param pScene assimpシーンへのポインタ
	 * @param directory ディレクトリパス
	 */
	void CreateMaterials(const aiScene* pScene, const std::string& directory);
};
