//Vector2.cpp
#include <Utility/Vector2.h>
#include <Utility/MathUtil.h>
#include <math.h>

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

float Vector2::Magnitude() const
{
	return sqrtf(x * x + y * y);
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

float Vector2::ToAngle()
{
	//x‚Æy‚ğ“ü‚ê‘Ö‚¦‚ÄŒv‰ñ‚è‚Å‹‚ß‚é
	float rad = atan2f(x, y);
	float angle = MathUtil::RadToDeg(rad);
	angle = MathUtil::NormalizeAngle(angle);
	return angle;
}
