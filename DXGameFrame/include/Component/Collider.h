/*****************************************************************//**
 * @file   Collider.h
 * @brief  当たり判定コンポーネント(回転させれる直方体のみ)
 *
 * @author 三品怜
 * @date   2025/11/13
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>
#include <Utility/Utility.h>
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

	/**
	 * @brief ★当たり判定の回転をオイラー角で取得する
	 * @return 現在の当たり判定の回転 (オイラー角)
	 */
	Vector3 GetEulerAngle()
	{
		return m_eulerOffset;
	}

	/**
	 * @brief ★当たり判定を移動させる(ローカル)
	 * @param translation 当たり判定移動量
	 */
	void Translate(Vector3 translation)
	{
		m_positionOffset += translation;
	}

	/**
	 * @brief ★当たり判定を回転させる(ローカル)
	 * @param euler 回転量 (オイラー)
	 */
	void Rotate(Vector3 euler)
	{
		m_eulerOffset += euler;
		m_rotateOffset = Quaternion::Euler(m_eulerOffset);
	}

	/**
	 * @brief ★当たり判定を回転させる(ローカル)
	 * @param x x軸回転
	 * @param y y軸回転
	 * @param z z軸回転
	 */
	void Rotate(float x, float y, float z)
	{
		m_eulerOffset.x += x;
		m_eulerOffset.y += y;
		m_eulerOffset.z += z;
		m_rotateOffset = Quaternion::Euler(m_eulerOffset);
	}

	/**
	 * @brief ★当たり判定の回転をオイラー角で設定する(ローカル)
	 * @param euler 設定する回転 (オイラー角)
	 */
	void SetEulerAngle(Vector3 euler)
	{
		m_eulerOffset = euler;
		m_rotateOffset = Quaternion::Euler(m_eulerOffset);
	}

	/**
	 * @brief オブジェクトの回転をクォータニオンで設定する(ローカル)
	 * @param quaternion 設定する回転 (クォータニオン)
	 */
	void SetQuaternion(Quaternion quaternion)
	{
		m_rotateOffset = quaternion;
		m_eulerOffset = m_rotateOffset.ToEuler();
	}

	// ローカル座標
	Vector3 m_positionOffset;
	Vector3 m_scale;
	std::function<void(GameObject* other)> OnCollisionEnter;

private:

	// ローカル回転
	Vector3 m_eulerOffset;

	// ローカル回転
	Quaternion m_rotateOffset;
};
