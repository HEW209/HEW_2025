/*********************************************************************
 * @file   VecUtil.h
 * @brief  Vec関連のユーティリティー関数
 *
 * @author 芝晃佑
 * @date   2025/11/16
 *********************************************************************/

#pragma once

#include <DXGameFrame.h>

#include "Vec.h"


// クォータニオンから回転行列の各軸ベクトルを取得する
void GetBasisFromQuaternion(const Quaternion& q, Vec3& right, Vec3& up, Vec3& forward);

// 回転行列（基底ベクトル）からクォータニオンへの変換
Quaternion BasisToQuaternion(const Vec3& r, const Vec3& u, const Vec3& f);

// ベクトルを最も近い基本軸にスナップさせる
Vec3 SnapToCardinalAxis(const Vec3& v);	

/**
 * @brief 現在の回転を最寄りの90度単位の回転に補正する
 * @param rotation 現在のクォータニオン
 * @return 補正後のクォータニオン
 */
Quaternion SnapRotationToNearest90(const Quaternion& rotation);

Vec2 GetFlattenedDirection(const Quaternion& rotation);

Vector3 LerpVector3(const Vector3& from, const Vector3& to, float t);

Quaternion NlerpQuaternion(const Quaternion& from, const Quaternion& to, float t);
Quaternion SlerpQuaternion(const Quaternion& from, const Quaternion& to, float t);