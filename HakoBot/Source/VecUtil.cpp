// VecUtil.cpp

#include "VecUtil.h"

void GetBasisFromQuaternion(const Quaternion& q, Vec3& right, Vec3& up, Vec3& forward) {
    float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
    float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
    float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
    float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;

    right = { 1.0f - (yy + zz), xy + wz, xz - wy };
    up = { xy - wz, 1.0f - (xx + zz), yz + wx };
    forward = { xz + wy, yz - wx, 1.0f - (xx + yy) };
}

[[nodiscard]] Quaternion BasisToQuaternion(const Vec3& r, const Vec3& u, const Vec3& f) {
    Quaternion q;
    float trace = r.x + u.y + f.z;

    if (trace > 0.0f) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        q.w = 0.25f / s;
        q.x = (u.z - f.y) * s;
        q.y = (f.x - r.z) * s;
        q.z = (r.y - u.x) * s;
    }
    else {
        if (r.x > u.y && r.x > f.z) {
            float s = 2.0f * std::sqrt(1.0f + r.x - u.y - f.z);
            q.w = (u.z - f.y) / s;
            q.x = 0.25f * s;
            q.y = (u.x + r.y) / s;
            q.z = (f.x + r.z) / s;
        }
        else if (u.y > f.z) {
            float s = 2.0f * std::sqrt(1.0f + u.y - r.x - f.z);
            q.w = (f.x - r.z) / s;
            q.x = (u.x + r.y) / s;
            q.y = 0.25f * s;
            q.z = (f.y + u.z) / s;
        }
        else {
            float s = 2.0f * std::sqrt(1.0f + f.z - r.x - u.y);
            q.w = (r.y - u.x) / s;
            q.x = (f.x + r.z) / s;
            q.y = (f.y + u.z) / s;
            q.z = 0.25f * s;
        }
    }
    return q;
}

[[nodiscard]] Vec3 SnapToCardinalAxis(const Vec3& v) {
    float absX = std::abs(v.x);
    float absY = std::abs(v.y);
    float absZ = std::abs(v.z);

    // 最大成分だけを残して他を0にする
    if (absX > absY && absX > absZ) {
        return { (v.x > 0) ? 1.0f : -1.0f, 0.0f, 0.0f };
    }
    else if (absY > absZ) {
        return { 0.0f, (v.y > 0) ? 1.0f : -1.0f, 0.0f };
    }
    else {
        return { 0.0f, 0.0f, (v.z > 0) ? 1.0f : -1.0f };
    }
}

[[nodiscard]] Quaternion SnapRotationToNearest90(const Quaternion& rotation) {
    Quaternion normalizedRotation = rotation.Normalized();

    Vec3 right, up, forward;

    // 現在の回転から基底ベクトルを取得
    GetBasisFromQuaternion(normalizedRotation, right, up, forward);

    // Forwardベクトルを最も近い基本軸にスナップ
    Vec3 snappedForward = SnapToCardinalAxis(forward);

    // Upベクトルを最も近い基本軸にスナップ
    Vec3 bestUp = SnapToCardinalAxis(up);

    // UpとForwardが平行の場合、Rightを使用してUpを再計算
    if (std::abs(snappedForward.Dot(bestUp)) > 0.9f) {
        Vec3 snappedRight = SnapToCardinalAxis(right);

        // RightとForwardが平行の場合、Identityを返す
        if (std::abs(snappedForward.Dot(snappedRight)) > 0.9f) {
            return { 0, 0, 0, 1 };
        }

        // RightとForwardからUpを再計算
        bestUp = snappedForward.Cross(snappedRight).Normalized();
    }

    // ForwardとUpから直交基底を作成
    Vec3 finalRight = bestUp.Cross(snappedForward).Normalized();
    Vec3 finalUp = snappedForward.Cross(finalRight).Normalized();

    // 直交基底からクォータニオンを作成
    return BasisToQuaternion(finalRight, finalUp, snappedForward);
}

[[nodiscard]] Vec2 GetFlattenedDirection(const Quaternion& rotation)
{
    static const Vec3 localForward = { 0.0f, 0.0f, 1.0f };

    Vec3 worldForward = rotation * localForward;

    Vec2 flatDir = { worldForward.x, worldForward.z };

    if (flatDir.LengthSq() < flatDir.EpsilonScalar) {
        return { 0.0f, 1.0f };
    }

    return flatDir.Normalized();
}
