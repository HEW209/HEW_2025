//Color.cpp
#include <Utility/Color.h>
#include <math.h>

const Color Color::black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::white(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::red(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::green(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::blue(0.0f, 0.0f, 1.0f, 1.0f);

Color::Color() :
	Color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Color::Color(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

void Color::Clamp01(float red, float green, float blue, float alpha)
{
	r = red * RGB255_InvMax;
	g = green * RGB255_InvMax;
	b = blue * RGB255_InvMax;
	a = alpha * RGB255_InvMax;
}

Color Color::Normalized()
{
	Color result = *this;

	//ç≈è¨ílï‚ê≥
	result.r = fmaxf(result.r, 0.0f);
	result.g = fmaxf(result.g, 0.0f);
	result.b = fmaxf(result.b, 0.0f);
	result.a = fmaxf(result.a, 0.0f);

	//ç≈ëÂílï‚ê≥
	result.r = fminf(result.r, 1.0f);
	result.g = fminf(result.g, 1.0f);
	result.b = fminf(result.b, 1.0f);
	result.a = fminf(result.a, 1.0f);

	return result;
}
