/******************************************************************//**
 * @file   Geometry.h
 * @brief  基本図形の生成を行う
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "Model.h"
#include <array>
#include <memory>

/**
 * @brief 基本図形の生成を行う
 */
class Geometry
{
public:
	/// ジオメトリタイプ
	enum Type
	{
		BOX,		// 箱
		CYLINDER,	// 円柱
		SHPERE,		// 球
		PLANE,		// 板
		COUNT
	};

	/**
	 * @brief ジオメトリの初期化
	 * @return 成功したかを返す
	 */
	HRESULT Init();

	/**
	 * @brief 終了処理
	 */
	void Uninit();

	/**
	 * @brief ジオメトリモデルを取得する
	 */
	std::shared_ptr<Model> GetModel(Type geometryType);

private:
	Geometry() = default;

	/// ジオメトリモデル配列
	std::array<std::shared_ptr<Model>, Type::COUNT> m_pModels;

	/**
	 * @brief 箱のメッシュを生成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateBox();

	/**
	 * @brief 円柱のメッシュを生成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateCylinder();

	/**
	 * @brief 球のメッシュを生成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateSphere();

	/**
	 * @brief 板のメッシュを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreatePlane();

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return Geometryインスタンスへの参照
	 */
	static Geometry& Instance();
};
