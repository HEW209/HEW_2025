/******************************************************************//**
 * @file   Vector3.h
 * @brief  三次元ベクトルを扱う
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include <DirectX/DirectXInclude.h>

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
	Vector3& operator=(const Vector3& other);
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	//加算・減算
	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);

	//スカラー
	Vector3 operator*(float scalar) const;
	Vector3 operator/(float scalar) const;
	Vector3& operator*=(float scalar);
	Vector3& operator/=(float scalar);

	/**
	 * @brief xyz成分をセットする
	 * @param newX セットするx成分
	 * @param newY セットするy成分
	 * @param newZ セットするz成分
	 */
	void SetVector(float newX, float newY, float newZ);

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

	/**
	 * @brief XMVECTOR型に変換する
	 * @return Vector3から変換されたXMVECTOR
	 */
	DirectX::XMVECTOR ToXMVector() const;

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