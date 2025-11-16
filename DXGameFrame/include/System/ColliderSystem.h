/*****************************************************************//**
 * @file   ColliderSystem.h
 * @brief  当たり判定コンポーネント(回転させれる直方体のみ)を管理する
 *
 * @author 三品怜
 * @date   2025/11/16
 *********************************************************************/
#pragma once

#include <Component/Collider.h>

/**
 * @brief 全ての当たり判定を管理するシステム
 */
class ColliderSystem
{
public:
	/**
	 * @brief 全ての当たり判定コンポーネントの描画処理を呼び出す
	 */
	void Check();

	/**
	 * @brief 3D当たり判定コンポーネントを登録する
	 * @param pCollider 登録する3D当たり判定画コンポーネント
	 */
	void Register(Collider* pCollider);

	/**
	 * @brief 3D当たり判定コンポーネントの登録を削除する
	 * @param pCollider 登録削除する3D当たり判定コンポーネント
	 */
	void Unregister(Collider* pCollider);

private:

	ColliderSystem() {};

	/**
	 * @brief 軸に対する射影半径を計算する
	 * @param Vector3 当たり判定の大きさ
	 * @param Vector3 射影する軸
	 * @param Collider::Axis3 当たり判定のx,y,z軸のベクトル
	 * @return Vector3
	 */
	double GetProjectionRadius(const Vector3 scale, const Vector3 axis, const Collider::Axis3 obbAxes);

	/**
	 * @brief OBB同士の衝突判定 (SAT)
	 * @param a OBB A
	 * @param b OBB B
	 * @return 衝突していれば true
	 */
	bool CheckCollisionOBB(Collider::ObbData data, Collider::ObbData otherData,Vector3* pMtv);

	/**
	 * @brief Vector3の内積
	 * @param Vector3
	 * @param Vector3
	 * @return Vector3 内積
	 */
	float Dot(Vector3 v, Vector3 other);

	/**
	 * @brief Vector3の外積
	 * @param Vector3
	 * @param Vector3
	 * @return Vector3 外積
	 */
	Vector3 Cross(Vector3 v, Vector3 other);

	/// 3D描画コンポーネントのリスト
	std::vector<Collider*> m_colliders;

public:

	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return RenderSystemインスタンスへの参照
	 */
	static ColliderSystem& Instance()
	{
		static ColliderSystem s_instance;
		return s_instance;
	}
};