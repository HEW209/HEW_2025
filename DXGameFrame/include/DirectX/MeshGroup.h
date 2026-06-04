/******************************************************************//**
 * @file   MeshGroup.h
 * @brief  モデル単位のメッシュの組み合わせ
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "DXTypes.h"
#include "Mesh.h"
#include "Material.h"
#include <assimp/scene.h>
#include <map>

/**
 * @brief モデル単位のメッシュの組み合わせ
 */
class MeshGroup
{
public:
	MeshGroup();
	~MeshGroup() = default;

	using AnimeNo = int;
	static const AnimeNo ANIME_NONE = -1;	// 該当アニメーションなし
	static const AnimeNo PARAMETRIC_ANIME = -2;	// 合成アニメーション

	/**
	 * @brief メッシュグループを作成する
	 * @param pScene assimpのシーンへのポインタ
	 */
	void Create(const aiScene* pScene, ModelLoadType loadType = ModelLoadType::DEFAULT);

	/**
	 * @brief メッシュ情報から単一のメッシュを作成する
	 * @param desc メッシュ情報
	 * @return 成功したかを返す
	 */
	HRESULT Create(const Mesh::Description& desc);

	/*
	 * @brief アニメーションデータの追加読み込み
	 * @param[in] file 読み込み先パス
	 * @return アニメーション番号
	 */
	AnimeNo AddAnimation(const aiScene* pScene);

	/*
	 * @brief アニメーションの再生
	 * @param[in] no 再生するアニメーション番号
	 * @param[in] loop ループ再生フラグ
	 * @param[in] speed 再生速度
	 */
	void PlayAnime(AnimeNo no, bool loop, float speed = 1.0f);

	/*
	 * @brief ブレンド再生
	 * @param[in] no アニメーション番号
	 * @param[in] blendTime ブレンドに掛ける時間
	 * @param[in] loop ループフラグ
	 * @param[in] speed 再生速度
	 */
	void PlayBlend(AnimeNo no, float blendTime, bool loop, float speed = 1.0f);

	/*
	 * @brief アニメーションの合成再生
	 * @param[in] no1 合成元１
	 * @param[in] no2 合成元２
	 */
	void SetParametric(AnimeNo no1, AnimeNo no2);

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
	void SetAnimeTime(AnimeNo no, float time);

	/**
	 * @brief メッシュグループの描画処理
	 * @param materials 使用するマテリアル配列
	 */
	void Draw(const std::vector<Material>& materials);

	// アニメーションの更新
	void StepAnime(float tick);

	using BoneIndex = int;
	static const BoneIndex BONE_NONE = -1;

	/// ボーン情報
	struct Bone
	{
		std::string name;						// ボーン名
		BoneIndex parentID;						// 親ボーンID
		std::vector<BoneIndex> childrenIDs;		// 子ボーンID配列
		DirectX::XMMATRIX matrix;				// 変換行列
	};

	int GetMeshNum();
	Mesh* GetMesh(int index);
	DirectX::XMMATRIX GetBoneMatrix(BoneIndex index);

private:
	// アニメーション再生方法
	enum AnimePattern
	{
		MAIN,			// 通常再生
		BLEND,			// ブレンド再生
		PARAMETRIC0,	// 合成A
		PARAMETRIC1,	// 合成B
		MAX_ANIMEPATTERN
	};

	// アニメーション再生情報
	struct AnimePlayInfo
	{
		float nowTime;		// 現在の再生時間
		float totalTime;	// 最大再生時間
		float speed;		// 再生速度
		bool isLoop;		// ループ指定
	};

	// アニメーションの変換情報
	struct AnimeTransform
	{
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4	quaternion;
		DirectX::XMFLOAT3	scale;
	};

	using AnimeKey = std::pair<float, AnimeTransform>;
	using AnimeTimeline = std::map<float, AnimeTransform>;
	using AnimeTransforms = std::vector<AnimeTransform>;

	// アニメーションとノードの関連付け情報
	struct AnimeChannel
	{
		BoneIndex bone;		// 対応するボーン
		AnimeTimeline timeline;	// ボーンに付随するアニメーション情報
	};
	using AnimeChannels = std::vector<AnimeChannel>;

	// アニメーション情報
	struct Animation
	{
		AnimePlayInfo info;		// 再生情報
		AnimeChannels channels;	// 変換情報
	};
	using Animations = std::vector<Animation>;

	/// メッシュ配列
	std::vector<Mesh> m_meshes;

	/// ボーン配列
	std::vector<Bone> m_bones;

	/// アニメーション再生方法別変形情報
	AnimeTransforms	m_animeTransform[MAX_ANIMEPATTERN];

	std::vector<Animation> m_animes;
	AnimeNo m_playNo;			// 現在再生中のアニメ番号
	AnimeNo m_blendNo;			// ブレンド再生を行うアニメ番号
	AnimeNo m_parametric[2];	// 合成再生を行うアニメ番号
	float m_blendTime;			// 現在の遷移経過時間
	float m_blendTotalTime;		// アニメ遷移にかかる合計時間
	float m_parametricBlend;	// パラメトリックの再生割合

	/**
	 * @brief メッシュを作成する
	 * @param pScene assimpシーンへのポインタ
	 */
	void CreateMesh(const aiScene* pScene, ModelLoadType loadType);

	/**
	 * @brief ボーンを作成する
	 * @param pScene assimpシーンへのポインタ
	 */
	void CreateBones(const aiScene* pScene);

	// ボーン探索
	BoneIndex FindNode(const char* name);

	// 頂点ブレンドの作成
	void MakeVertexWeight(const aiScene* pScene, int meshIndex, Mesh::Description& desc);
	// ボーンありの頂点ブレンド作成
	void MakeVertexWeightHasBone(const aiMesh* pMesh, Mesh::Description& desc);
	// 親子関係をもとに頂点ブレンド作成
	void MakeVertexWeightFromNode(const aiScene* pScene, const aiMesh* pMesh, Mesh::Description& desc);

	// アニメーションの初期化
	void InitAnime(AnimeNo no);
	// アニメーションの再生情報の更新
	void UpdateAnime(AnimeNo no, float tick);
	// アニメーションの姿勢行列を計算
	void CalcAnime(AnimePattern kind, AnimeNo no);
	// 再生方法別の行列結果の合成
	void CalcBones(BoneIndex index, const DirectX::XMMATRIX parent);
	// 行列結果同士の補間
	void LerpTransform(AnimeTransform* pOut, const AnimeTransform& a, const AnimeTransform& b, float rate);
	// アニメーションのループチェック
	void CheckAnimePlayLoop(AnimePlayInfo& info);
	// アニメーション番号のチェック
	bool CheckAnimeNo(AnimeNo no);
};
