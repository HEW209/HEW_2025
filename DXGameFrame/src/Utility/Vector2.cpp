// Vector2.cpp
#include <Utility/Vector2.h>
#include <Utility/Math.h>
#include <cmath>
#include <cassert>

const Vector2 Vector2::zero(0.0f, 0.0f);
const Vector2 Vector2::one(1.0f, 1.0f);
const Vector2 Vector2::up(0.0f, 1.0f);
const Vector2 Vector2::down(0.0f, -1.0f);
const Vector2 Vector2::right(1.0f, 0.0f);
const Vector2 Vector2::left(-1.0f, 0.0f);

Vector2::Vector2() :
	Vector2(Vector2::zero)
{
}

Vector2::Vector2(float x, float y) :
	x(x), y(y)
{
}

Vector2& Vector2::operator=(const Vector2& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
	return Math::Approximately(x, other.x) &&
		Math::Approximately(y, other.y);
}

bool Vector2::operator!=(const Vector2& other) const
{
	return !(*this == other);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	*this = *this + other;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	*this = *this - other;
	return *this;
}

Vector2 Vector2::operator*(float scalar) const
{
	return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const
{
	// ゼロ除算チェック
	if (scalar == 0.0f)
	{
		// Debug中は終了
		assert(false && "Vector2のゼロ除算");

		// 元の値を返す
		return *this;
	}

	return Vector2(x / scalar, y / scalar);
}

Vector2& Vector2::operator*=(float scalar)
{
	*this = *this * scalar;
	return *this;
}

Vector2& Vector2::operator/=(float scalar)
{
	*this = *this / scalar;
	return *this;
}

void Vector2::SetVector(float newX, float newY)
{
	x = newX;
	y = newY;
}

float Vector2::Magnitude() const
{
	return std::sqrtf(x * x + y * y);
}

Vector2 Vector2::Normalized() const
{
	float mag = Magnitude();

	if (mag == 0.0f)
	{
		return Vector2::zero;
	}
	else
	{
		float invMag = 1.0f / mag;
		return *this * invMag;
	}
}

float Vector2::ToAngle() const
{
	// xとyを入れ替えて時計回りで求める
	float rad = std::atan2f(x, y);
	float angle = Math::RadToDeg(rad);
	return angle;
}

DirectX::XMVECTOR Vector2::ToXMVector() const
{
	return DirectX::XMVectorSet(x, y, 0.0f, 1.0f);
}