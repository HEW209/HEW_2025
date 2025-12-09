/*****************************************************************//**
 * @file   Vector2.h
 * @brief  二次元ベクトルを扱う
 *
 * @author 石田怜
 * @date   2025/09/10
 *********************************************************************/
#pragma once

#include "MathUtil.h"
#include <DirectX/DirectXInclude.h>
#include <cassert>

/**
 * @brief 二次元ベクトルを扱う
 */
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2() = default;

	/// x成分
	float x;

	/// y成分
	float y;

	//比較・代入
	Vector2& operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
	bool operator==(const Vector2& other) const
	{
		return MathUtil::Approximately(x, other.x) && MathUtil::Approximately(y, other.y);
	}
	bool operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	//加算・減算
	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2& operator+=(const Vector2& other)
	{
		*this = *this + other;
		return *this;
	}
	Vector2& operator-=(const Vector2& other)
	{
		*this = *this - other;
		return *this;
	}

	//スカラー
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}
	Vector2 operator/(float scalar) const
	{
		//ゼロ除算チェック
		assert(scalar != 0.0f);				//Debugでは強制終了
		if (scalar == 0.0f)
			return Vector2::zero;			//Releaseではゼロベクトルを返す

		return Vector2(x / scalar, y / scalar);
	}
	Vector2& operator*=(float scalar)
	{
		*this = *this * scalar;
		return *this;
	}
	Vector2& operator/=(float scalar)
	{
		*this = *this / scalar;
		return *this;
	}

	/**
	 * @brief 新しいベクター成分をセットする
	 * @param newX セットするx成分
	 * @param newY セットするy成分
	 */
	void SetVector(float newX, float newY)
	{
		x = newX;
		y = newY;
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
	Vector2 Normalized() const;

	/**
	 * @brief ベクトルを角度に変換する
	 * @return 0を真上とした時計回りの角度(0～360)
	 */
	float ToAngle() const;

	/**
	 * @brief XMVECTOR型に変換する
	 * @return Vector2から変換されたXMVECTOR
	 */
	DirectX::XMVECTOR ToXMVector() const;

public:
	/// 大きさが0のベクトル
	static const Vector2 zero;

	/// 全ての成分の大きさが1のベクトル
	static const Vector2 one;

	/// 上向きの正規化ベクトル
	static const Vector2 up;

	/// 下向きの正規化ベクトル
	static const Vector2 down;

	/// 右向きの正規化ベクトル
	static const Vector2 right;

	/// 左向きの正規化ベクトル
	static const Vector2 left;
};