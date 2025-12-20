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

	// レイ用構造体
	struct Ray {
		Vector3 origin;
		Vector3 direction;
	};

	// レイが当たった時の情報
	struct RaycastHit {
		Vector3 point;    // 当たった座標
		float distance;   // 距離
		Vector3 normal;   // 当たった面の法線
		GameObject* hitObj;// 当たったオブジェクト
	};

	/**
	 * @brief 全ての当たり判定コンポーネントを当たってるかチェック
	 */
	void Check();

	/**
	 * @brief 全ての当たり判定コンポーネントとレイが当たってるかチェック
	 * @param ray チェックするレイ
	 * @param RaycastHit 当たった時に情報を入れる構造体のポインター
	 * @param maxDistance レイの長さ
	 */
	bool Raycast(const Ray& ray, RaycastHit* outHit, float maxDistance);

	/**
	 * @brief 3D当たり判定コンポーネントを登録する
	 * @param pCollider 登録する3D当たり判定コンポーネント
	 */
	void Register(Collider* pCollider);

	/**
	 * @brief 3D当たり判定コンポーネントの登録を削除する
	 * @param pCollider 登録削除する3D当たり判定コンポーネント
	 */
	void Unregister(Collider* pCollider);

#ifdef _DEBUG

	void SetDrawFlag(bool flag);

#endif

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
	bool CheckCollisionOBB(Collider::ObbData data, Collider::ObbData otherData, Vector3* pMtv);

	/**
	 * @brief OBBとrayの当たり判定
	 * @return 衝突していれば true
	 */
	bool IntersectRayObb(const Ray& ray, const Collider::ObbData& obb, float& tMin, float& tMax, Vector3& outNormal);

public:

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

private:

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