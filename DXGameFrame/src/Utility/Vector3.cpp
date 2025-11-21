//Vector3.cpp
#include <Utility/Vector3.h>
#include <Utility/MathUtil.h>
#include <math.h>

const Vector3 Vector3::zero		( 0.0f,  0.0f,  0.0f);
const Vector3 Vector3::one		( 1.0f,  1.0f,  1.0f);
const Vector3 Vector3::forward	( 0.0f,  0.0f,  1.0f);
const Vector3 Vector3::back		( 0.0f,  0.0f, -1.0f);
const Vector3 Vector3::up		( 0.0f,  1.0f,  0.0f);
const Vector3 Vector3::down		( 0.0f, -1.0f,  0.0f);
const Vector3 Vector3::right	( 1.0f,  0.0f,  0.0f);
const Vector3 Vector3::left		(-1.0f,  0.0f,  0.0f);

Vector3::Vector3() :
	Vector3(Vector3::zero)
{
}

Vector3::Vector3(float x, float y, float z) :
	x(x), y(y), z(z)
{
}

float Vector3::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z);
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