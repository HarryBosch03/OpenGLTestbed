#include "ColorUtil.h"

#include "Maths.h"

using glm::max;
using glm::min;

const unsigned int mask = 0xFF;

float FromHex1(unsigned int code, int offset)
{
	return ((code >> offset) & mask) / 255.0f;
}

void ColorUtil::FromHex3(unsigned int code, float* r, float* g, float* b)
{
	*r = FromHex1(code, 16);
	*g = FromHex1(code, 8);
	*b = FromHex1(code, 0);
}

void ColorUtil::FromHex4(unsigned int code, float* r, float* g, float* b, float* a)
{
	*r = FromHex1(code, 24);
	FromHex3(code, g, b, a);
}

void ColorUtil::FromHSV(float h, float s, float v, float* r, float* g, float* b)
{
	if (v <= 0)
	{
		*r = 0; *g = 0; *b = 0;
		return;
	}

	h = h > 0.0f ? fmod(h, 1.0f) : 1.0f + fmod(h, 1.0f);

	float c = v * s;
	float hp = h * 6.0f;
	float x = c * (1.0f - abs(fmod(hp, 2.0f) - 1.0f));

	if (hp < 1)
	{
		*r = c;
		*g = x;
		*b = 0.0f;
	}
	else if (hp < 2)
	{
		*r = x;
		*g = c;
		*b = 0.0f;
	}
	else if (hp < 3)
	{
		*r = 0.0f;
		*g = c;
		*b = x;
	}
	else if (hp < 4)
	{
		*r = 0.0f;
		*g = x;
		*b = c;
	}
	else if (hp < 5)
	{
		*r = x;
		*g = 0.0f;
		*b = c;
	}
	else
	{
		*r = c;
		*g = 0.0f;
		*b = x;
	}
}

void ColorUtil::ToHSV(float r, float g, float b, float *h, float *s, float *v)
{
	*v = max(r, max(g, b));
	float xMin = min(r, min(g, b));
	float c = *v - xMin;

	*s = *v > 0.0f ? c / *v : 0.0f;

	if (c == 0) *h = 0.0f;
	else if (*v == r) *h = fmod((g - b) / c, 6.0f) / 6.0f;
	else if (*v == g) *h = ((b - r) / c + 2.0f) / 6.0f;
	else if (*v == b) *h = ((r - g) / c + 4.0f) / 6.0f;
}

Vec4 ColorUtil::FromHex3(unsigned int code)
{
	Vec4 col;
	FromHex3(code, &col.r, &col.g, &col.b); col.a = 1.0f;
	return col;
}

Vec4 ColorUtil::FromHex4(unsigned int code)
{
	Vec4 col;
	FromHex4(code, &col.r, &col.g, &col.b, &col.a);
	return col;
}

Vec4 ColorUtil::FromHSV(float h, float s, float v)
{
	Vec4 col;
	FromHSV(h, s, v, &col.r, &col.g, &col.b); col.a = 1.0f;
	return col;
}

Vec4 ColorUtil::ToHSV(float r, float g, float b)
{
	Vec4 col;
	FromHSV(r, g, b, &col.r, &col.g, &col.b); col.a = 1.0f;
	return col;
}