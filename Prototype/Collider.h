/*****************************************************************//**
 * @file   Collider.h
 * @brief  当たり判定コンポーネント
 *
 * @author 三品怜
 * @date   2025/11/13
 *********************************************************************/
#pragma once

#include <DXGameFrame.h>
#include <vector>

#include <functional>

class Collider :public Component
{
public:

	struct Axis3
	{
		Vector3 x, y, z;
	};

	struct ObbData
	{
		Vector3 pos;
		Vector3 scale;
		Axis3 axis;
	};

	Collider();
	~Collider();


	Quaternion GetQuaternion() { return m_rotateOffset; }
	Vector3 GetPosition() { return m_positionOffset; }
	Vector3 GetScale() { return m_scale; }

	Vector3 m_positionOffset;
	Vector3 m_scale;
	std::function<void(GameObject* other)> onCollision;

private:
	
	Quaternion m_rotateOffset;
};

void CheckCollision();

double GetProjectionRadius(const Vector3 scale, const Vector3 axis, const Collider::Axis3 obbAxes);

float Dot(Vector3 v, Vector3 other);

/**
 * @brief OBB同士の衝突判定 (SAT)
 * @param a OBB A
 * @param b OBB B
 * @return 衝突していれば true
 */
bool CheckCollisionOBB(Collider::ObbData data, Collider::ObbData otherData);

Vector3 Cross(Vector3 v, Vector3 other);