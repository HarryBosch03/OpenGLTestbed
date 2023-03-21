#pragma once

#include "Maths.h"

namespace ColorUtil
{
	void FromHex3(unsigned int code, float* r, float* g, float* b);
	void FromHex4(unsigned int code, float* r, float* g, float* b, float* a);
	void FromHSV(float h, float s, float v, float* r, float* g, float* b);
	void ToHSV(float h, float s, float v, float* r, float* g, float* b);

	Vec4 FromHex3(unsigned int code);
	Vec4 FromHex4(unsigned int code);
	Vec4 FromHSV(float h, float s, float b);
	Vec4 ToHSV(float r, float g, float b);
}