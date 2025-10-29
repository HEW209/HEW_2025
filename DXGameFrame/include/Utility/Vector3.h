/*****************************************************************//**
 * @file   Vector3.h
 * @brief  三次元ベクトルを扱う
 *
 * @author 石田怜
 * @date   2025/09/15
 *********************************************************************/
#pragma once

#include "MathUtil.h"
#include <cassert>

 /**
  * @brief 三次元ベクトルを扱う
  */
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	~Vector3() = default;

	/// x成分
	float x;

	/// y成分
	float y;

	/// z成分
	float z;

	//比較・代入
	Vector3& operator=(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	bool operator==(const Vector3& other) const
	{
		return MathUtil::Approximately(x, other.x) &&
			MathUtil::Approximately(y, other.y) &&
			MathUtil::Approximately(z, other.z);
	}
	bool operator!=(const Vector3& other) const
	{
		return !(*this == other);
	}

	//加算・減算
	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3& operator+=(const Vector3& other)
	{
		*this = *this + other;
		return *this;
	}
	Vector3& operator-=(const Vector3& other)
	{
		*this = *this - other;
		return *this;
	}

	//スカラー
	Vector3 operator*(float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}
	Vector3 operator/(float scalar) const
	{
		assert(scalar != 0.0f);				//Debugでは強制終了
		if (scalar == 0.0f)
			return Vector3::zero;			//Releaseではゼロベクトルを返す

		return Vector3(x / scalar, y / scalar, z / scalar);
	}
	Vector3& operator*=(float scalar)
	{
		*this = *this * scalar;
		return *this;
	}
	Vector3& operator/=(float scalar)
	{
		*this = *this / scalar;
		return *this;
	}

	/**
	 * @brief ベクトルの長さを取得する
	 * @return ベクトルの長さ
	 */
	float Magnitude() const;

	/**
	 * @brief 正規化ベクトルを取得する
	 * @return 正規化されたベクトルを返す
	 * ゼロベクトルを渡した場合はそのまま返す
	 */
	Vector3 Normalized() const;

public:
	/// 大きさが0のベクトル
	static const Vector3 zero;

	/// 全ての成分の大きさが1のベクトル
	static const Vector3 one;

	/// 前向きの正規化ベクトル
	static const Vector3 forward;

	/// 後ろ向きの正規化ベクトル
	static const Vector3 back;

	/// 上向きの正規化ベクトル
	static const Vector3 up;

	/// 下向きの正規化ベクトル
	static const Vector3 down;

	/// 右向きの正規化ベクトル
	static const Vector3 right;

	/// 左向きの正規化ベクトル
	static const Vector3 left;
};