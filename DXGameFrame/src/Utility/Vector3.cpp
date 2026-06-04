// Vector3.cpp
#include <Utility/Vector3.h>
#include <Utility/Math.h>
#include <cmath>
#include <cassert>

const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::back(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);

Vector3::Vector3() :
    Vector3(Vector3::zero)
{
}

Vector3::Vector3(float x, float y, float z) :
    x(x), y(y), z(z)
{
}

Vector3& Vector3::operator=(const Vector3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
    return Math::Approximately(x, other.x) &&
        Math::Approximately(y, other.y) &&
        Math::Approximately(z, other.z);
}

bool Vector3::operator!=(const Vector3& other) const
{
    return !(*this == other);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
    *this = *this + other;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
    *this = *this - other;
    return *this;
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const
{
    // É[ÉçèúéZÉ`ÉFÉbÉN
    if (scalar == 0.0f)
    {
        // DebugíÜÇÕèIóπ
        assert(false && "Vector3ÇÃÉ[ÉçèúéZ");

        // å≥ÇÃílÇï‘Ç∑
        return *this;
    }

    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3& Vector3::operator*=(float scalar)
{
    *this = *this * scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar)
{
    *this = *this / scalar;
    return *this;
}

void Vector3::SetVector(float newX, float newY, float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

float Vector3::Magnitude() const
{
    return std::sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized() const
{
    float mag = Magnitude();

    if (mag == 0.0f)
    {
        return Vector3::zero;
    }
    else
    {
        float invMag = 1.0f / mag;
        return *this * invMag;
    }
}

DirectX::XMVECTOR Vector3::ToXMVector() const
{
    return DirectX::XMVectorSet(x, y, z, 1.0f);
}