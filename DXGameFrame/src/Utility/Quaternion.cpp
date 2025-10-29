//Quaternion.cpp
#include <Utility/Quaternion.h>
#include <Utility/MathUtil.h>
#include <math.h>

const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion() :
    Quaternion(Quaternion::identity)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) :
    x(x), y(y), z(z), w(w)
{
}

Quaternion Quaternion::Normalized() const
{
    //クォータニオンの長さを求める
    float mag = sqrtf(x * x + y * y + z * z + w * w);
    if (mag == 0.0f)
        return Quaternion::identity;

    //正規化クォータニオンを求める
    float invMag = 1.0f / mag;
    return Quaternion(x * invMag, y * invMag, z * invMag, w * invMag);
}

Vector3 Quaternion::ToEuler() const
{
    Vector3 euler;

    //x軸回転(pitch)を求める
    float sinZ_cosX = 2.0f * (w * x + y * z);
    float cosZ_cosX = 1.0f - 2.0f * (x * x + y * y);
    euler.x = atan2f(sinZ_cosX, cosZ_cosX);

    //y軸回転(yaw)を求める
    float sinX = 2.0f * (w * y - z * x);
    if (fabsf(sinX) >= 1.0f)
    {
        euler.y = copysignf(Math::PI / 2.0f, sinX);
    }
    else
    {
        euler.y = asinf(sinX);
    }

    //z軸回転(roll)を求める
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    euler.z = atan2f(siny_cosp, cosy_cosp);

    //デグリーに変換
    euler.x = MathUtil::RadToDeg(euler.x);
    euler.y = MathUtil::RadToDeg(euler.y);
    euler.z = MathUtil::RadToDeg(euler.z);

    return euler;
}

Quaternion Quaternion::Euler(Vector3 euler)
{
    return Quaternion::Euler(euler.x, euler.y, euler.z);
}

Quaternion Quaternion::Euler(float x, float y, float z)
{
    //ラジアンに変換
    float rad_x = MathUtil::DegToRad(x);
    float rad_y = MathUtil::DegToRad(y);
    float rad_z = MathUtil::DegToRad(z);

    //sin・cosを求める
    float cx = cosf(rad_x * 0.5f);
    float sx = sinf(rad_x * 0.5f);
    float cy = cosf(rad_y * 0.5f);
    float sy = sinf(rad_y * 0.5f);
    float cz = cosf(rad_z * 0.5f);
    float sz = sinf(rad_z * 0.5f);

    //クォータニオンを求める
    //式は (qz * qx * qy) をまとめて展開したものです
    Quaternion q;
    q.w = cx * cy * cz + sx * sy * sz;
    q.x = sx * cy * cz - cx * sy * sz;
    q.y = cx * sy * cz + sx * cy * sz;
    q.z = cx * cy * sz - sx * sy * cz;
    return q.Normalized();
}