#pragma once

#include <string>

namespace Utility
{
	namespace Inspector
	{
		int& WorkingID();
		const char* AppendID(const std::string& text);
	}
}