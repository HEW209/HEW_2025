/******************************************************************//**
 * @file   Quaternion.h
 * @brief  クォータニオンを扱う
 *
 * 回転順はY→X→Zとして扱います
 * 参考：https://qiita.com/aa_debdeb/items/3d02e28fb9ebfa357eaf
 * 
 * @author 石田怜
 * @date   2025/09/23
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
	Quaternion operator=(const Quaternion& q);
	Quaternion operator*(const Quaternion& q) const;
	Quaternion operator*=(const Quaternion& q);

	//ベクトルとの演算
	Vector3 operator*(const Vector3& v) const;

	/**
	 * @brief 新しいクォータニオン成分をセットする
	 * @param newX セットするx成分
	 * @param newY セットするy成分
	 * @param newZ セットするz成分
	 * @param newW セットするw成分
	 */
	void SetQuaternion(float newX, float newY, float newZ, float newW);

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
	 * @brief 逆クォータニオンを取得
	 * @return このクォータニオンの逆クォータニオン
	 */
	Quaternion Inverse() const;

	/**
	 * @brief XMVECTOR型に変換する
	 * @return Quaternionから変換されたXMVECTOR
	 */
	DirectX::XMVECTOR ToXMVector() const;

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
