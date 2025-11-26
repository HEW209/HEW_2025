// Quaternion.cpp
#include <Utility/Quaternion.h>
#include <Utility/Math.h>
#include <cmath>

const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion() :
    Quaternion(Quaternion::identity)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) :
    x(x), y(y), z(z), w(w)
{
}

Quaternion Quaternion::operator=(const Quaternion& q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
    return Quaternion(
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
    );
}

Quaternion Quaternion::operator*=(const Quaternion& q)
{
    *this = *this * q;
    return *this;
}

Vector3 Quaternion::operator*(const Vector3& v) const
{
    Quaternion qv(v.x, v.y, v.z, 0);
    Quaternion inv(-x, -y, -z, w);
    Quaternion res = (*this) * qv * inv;
    return Vector3(res.x, res.y, res.z);
}

void Quaternion::SetQuaternion(float newX, float newY, float newZ, float newW)
{
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
}

Quaternion Quaternion::Normalized() const
{
    // クォータニオンの長さを求める
    float mag = sqrtf(x * x + y * y + z * z + w * w);
    if (mag == 0.0f)
        return Quaternion::identity;

    // 正規化クォータニオンを求める
    float invMag = 1.0f / mag;
    return Quaternion(x * invMag, y * invMag, z * invMag, w * invMag);
}

Vector3 Quaternion::ToEuler() const
{
    Vector3 euler;

    // pitch(X)
    float sinp = 2.0f * (w * x - y * z);
    float pitch;
    pitch = asinf(sinp);

    // yaw(Y)
    float yaw;
    if (fabsf(cos(pitch)) > Math::Epsilon)
    {
        float y1 = 2.0f * (x * z + y * w);
        float y2 = 2.0f * (w * w + z * z) - 1.0f;
        yaw = atan2f(y1, y2);
    }
    else
    {
        float y1 = -2.0f * (x * z - y * w);
        float y2 = 2.0f * (w * w + x * x) - 1.0f;
        yaw = atan2f(y1, y2);
    }

    // roll(Z)
    float roll;
    if (fabsf(cos(pitch)) > Math::Epsilon)
    {
        float z1 = 2.0f * (x * y + z * w);
        float z2 = 2.0f * (w * w + y * y) - 1.0f;
        roll = atan2f(z1, z2);
    }
    else
    {
        roll = 0.0f;
    }

    //デグリーに変換
    euler.x = Math::RadToDeg(pitch);
    euler.y = Math::RadToDeg(yaw);
    euler.z = Math::RadToDeg(roll);

    return euler;
}

Quaternion Quaternion::Inverse() const
{
    DirectX::XMVECTOR inv;
    inv = DirectX::XMQuaternionInverse(this->ToXMVector());

    DirectX::XMFLOAT4 f;
    DirectX::XMStoreFloat4(&f, inv);
    return Quaternion(f.x, f.y, f.z, f.w);
}

DirectX::XMVECTOR Quaternion::ToXMVector() const
{
    return DirectX::XMVectorSet(x, y, z, w);
}

Quaternion Quaternion::Euler(Vector3 euler)
{
    return Quaternion::Euler(euler.x, euler.y, euler.z);
}

Quaternion Quaternion::Euler(float x, float y, float z)
{
    // ラジアンに変換
    float rad_x = Math::DegToRad(x);
    float rad_y = Math::DegToRad(y);
    float rad_z = Math::DegToRad(z);

    // 計算に使用するsin・cosを求める
    float cx = cosf(rad_x * 0.5f);
    float sx = sinf(rad_x * 0.5f);
    float cy = cosf(rad_y * 0.5f);
    float sy = sinf(rad_y * 0.5f);
    float cz = cosf(rad_z * 0.5f);
    float sz = sinf(rad_z * 0.5f);

    // クォータニオンを求める
    // 回転の適用順はZ→X→YなのでY×X×Zになる
    Quaternion q;
    q.x = sx * cy * cz + cx * sy * sz;
    q.y = cx * sy * cz - sx * cy * sz;
    q.z = cx * cy * sz - sx * sy * cz;
    q.w = cx * cy * cz + sx * sy * sz;
    return q.Normalized();
}