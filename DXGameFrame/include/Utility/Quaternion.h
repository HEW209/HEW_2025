/*****************************************************************//**
 * @file   Quaternion.h
 * @brief  クォータニオンを扱う
 * 
 * @author 石田怜
 * @date   2025/09/15
 *********************************************************************/
#pragma once

#include "Vector3.h"
#include <DirectX/DirectXInclude.h>

/**
 * @brief クォータニオンを扱う
 */
class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	~Quaternion() = default;

	/// ベクトルx成分
	float x;

	/// ベクトルy成分
	float y;

	/// ベクトルz成分
	float z;

	/// スカラー成分
	float w;

	//クォータニオンとの演算
	Quaternion operator=(const Quaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		return *this;
	}
	Quaternion operator*(const Quaternion& q) const
	{
		return Quaternion(
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w,
			w * q.w - x * q.x - y * q.y - z * q.z
		);
	}
	Quaternion operator*=(const Quaternion& q)
	{
		*this = *this * q;
		return *this;
	}

	//ベクトルとの演算
	Vector3 operator*(const Vector3& v) const
	{
		Quaternion qv(v.x, v.y, v.z, 0);
		Quaternion inv(-x, -y, -z, w);
		Quaternion res = (*this) * qv * inv;
		return Vector3(res.x, res.y, res.z);
	}

	/**
	 * @brief 新しいクォータニオン成分をセットする
	 * @param newX セットするx成分
	 * @param newY セットするy成分
	 * @param newZ セットするz成分
	 * @param newW セットするw成分
	 */
	void SetQuaternion(float newX, float newY, float newZ, float newW)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	/**
	 * @brief 正規化クォータニオンを取得する
	 * @return 正規化されたクォータニオンを返す
	 */
	Quaternion Normalized() const;

	/**
	 * @brief クォータニオンからオイラー角を作成する
	 * @return 作成したオイラー角を返す
	 */
	Vector3 ToEuler() const;

	/**
	 * @brief XMVECTOR型に変換する
	 * @return Quaternionから変換されたXMVECTOR
	 */
	DirectX::XMVECTOR ToXMVector();

public:
	/// 回転無しクォータニオン
	static const Quaternion identity;

	/**
	 * @brief オイラー角からクォータニオンを作成する
	 * @param euler オイラー角
	 * @return 作成した正規化クォータニオンを返す
	 */
	static Quaternion Euler(Vector3 euler);

	/**
	 * @brief オイラー角からクォータニオンを作成する
	 * @param x x軸回転 (pitch)
	 * @param y y軸回転 (yaw)
	 * @param z z軸回転 (roll)
	 * @return 作成した正規化クォータニオンを返す
	 */
	static Quaternion Euler(float x, float y, float z);
};
