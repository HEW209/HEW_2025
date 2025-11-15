/*****************************************************************//**
 * @file   Transform.h
 * @brief  オブジェクトの座標・スケール・回転を表す
 * 
 * @author 石田怜
 * @date   2025/09/10
 *********************************************************************/
#pragma once

#include "Component.h"
#include <Utility/Vector3.h>
#include <Utility/Quaternion.h>
#include <DirectX/MatrixUtil.h>

/**
 * @brief オブジェクトの座標・スケール・回転を表す
 */
class Transform : public Component
{
public:
	Transform(GameObject* owner);
	~Transform() = default;

	/// オブジェクトの座標
	Vector3 m_position;

	/// オブジェクトのスケール
	Vector3 m_scale;

	/// 親Transformへのポインタ
	Transform* m_pParent;

	/// 子Transformへのポインタ
	std::vector<Transform*> m_pChildren;

	void SetParent(Transform* transform)
	{
		m_pParent = transform;
		transform->m_pChildren.emplace_back(this);
	}

	Vector3 GetWorldPosition();
	Vector3 GetWorldScale();
	Quaternion GetWorldQuaternion();

	DirectX::XMMATRIX GetWorldMatrix();

	/**
	 * @brief ★オブジェクトを移動させる
	 * @param translation オブジェクトの移動量
	 */
	void TransLate(Vector3 translation)
	{
		m_position += translation;
	}

	/**
	 * @brief ★オブジェクトを移動させる
	 * @param x オブジェクトのx移動量
	 * @param y オブジェクトのy移動量
	 * @param z オブジェクトのz移動量
	 */
	void TransLate(float x, float y, float z)
	{
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
	}

	/**
	 * @brief ★オブジェクトを回転させる
	 * @param euler 回転量 (オイラー)
	 */
	void Rotate(Vector3 euler)
	{
		m_euler += euler;
		m_quaternion = Quaternion::Euler(m_euler);
	}

	/**
	 * @brief ★オブジェクトを回転させる
	 * @param x x軸回転
	 * @param y y軸回転
	 * @param z z軸回転
	 */
	void Rotate(float x, float y, float z)
	{
		m_euler.x += x;
		m_euler.y += y;
		m_euler.z += z;
		m_quaternion = Quaternion::Euler(m_euler);
	}

	/**
	 * @brief ★オブジェクトの回転をオイラー角で取得する
	 * @return 現在のオブジェクトの回転 (オイラー角)
	 */
	Vector3 GetEulerAngle()
	{
		return m_euler;
	}

	/**
	 * @brief ★オブジェクトの回転をオイラー角で設定する
	 * @param euler 設定する回転 (オイラー角)
	 */
	void SetEulerAngle(Vector3 euler)
	{
		m_euler = euler;
		m_quaternion = Quaternion::Euler(m_euler);
	}

	/**
	 * @brief ★オブジェクトの回転をオイラー角で設定する
	 * @param x x軸回転
	 * @param y y軸回転
	 * @param z z軸回転
	 */
	void SetEulerAngle(float x, float y, float z)
	{
		m_euler.x = x;
		m_euler.y = y;
		m_euler.z = z;
		m_quaternion = Quaternion::Euler(m_euler);
	}

	/**
	 * @brief オブジェクトの回転をクォータニオンで取得する
	 * @return 現在のオブジェクトの回転 (クォータニオン)
	 */
	Quaternion GetQuaternion()
	{
		return m_quaternion;
	}

	/**
	 * @brief オブジェクトの回転をクォータニオンで設定する
	 * @param quaternion 設定する回転 (クォータニオン)
	 */
	void SetQuaternion(Quaternion quaternion)
	{
		m_quaternion = quaternion;
		m_euler = m_quaternion.ToEuler();
	}

private:
	//Transformクラスでは隠す
	using Component::SetEnabled;
	using Component::Destroy;

	/// オブジェクトのクォータニオン
	Quaternion m_quaternion;

	/// オブジェクトのオイラー角
	Vector3 m_euler;
};