#pragma once

#include "Maths.h"

namespace Utility
{
	namespace Color
	{
		void Hex3(unsigned int code, float* r, float* g, float* b);
		void Hex4(unsigned int code, float* r, float* g, float* b, float* a);
		void FromHSV(float h, float s, float v, float* r, float* g, float* b);
		void ToHSV(float h, float s, float v, float* r, float* g, float* b);

		Vec4 Hex3(unsigned int code);
		Vec4 Hex4(unsigned int code);
		Vec4 FromHSV(float h, float s, float b);
		Vec4 ToHSV(float r, float g, float b);

		Vec4 Grey(float v, float a = 1.0);
	}
}