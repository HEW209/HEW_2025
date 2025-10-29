/*****************************************************************//**
 * @file   Geometry.h
 * @brief  基本図形の生成を行う
 * 
 * @author 石田怜
 * @date   2025/10/18
 *********************************************************************/
#pragma once

#include <DirectX/Mesh.h>
#include <Utility/Common.h>

/**
 * @brief 基本図形の生成を行う
 */
class Geometry
{
public:
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
	 * @brief 箱のメッシュを取得する
	 */
	std::shared_ptr<Mesh> GetBox()
	{
		return m_pBoxMesh;
	}

private:
	Geometry() = default;

	/// 箱のメッシュ
	std::shared_ptr<Mesh> m_pBoxMesh;

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

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return Geometryインスタンスへの参照
	 */
	static Geometry& Instance()
	{
		static Geometry s_instance;
		return s_instance;
	}
};
