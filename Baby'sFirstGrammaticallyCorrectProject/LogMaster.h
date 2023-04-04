#pragma once

#include "FileUtility.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <sstream>

typedef std::string(*LogFormatter)(const std::string& in);

class LogMaster
{
	std::string Compile();
	std::stringstream stream;

	std::stringstream buffer;
	void PushLast();

public:
	void PushToConsole();
	void Test();

	std::stringstream& Message();
	std::stringstream& Warning();
	std::stringstream& Error();
	std::stringstream& Success();

	struct ErrorState
	{
		std::string message;

		ErrorState();
		ErrorState(const ErrorState& other) = delete;
		ErrorState& operator=(const ErrorState& other) = delete;
		~ErrorState();

		void Raise(const std::string& args);
		void Clear();

		inline bool Raised() { return message.size() > 0; }
	};
};

extern LogMaster Logger;

#define LogMessage(message) Logger.Message() << message << " [at " << Utility::Files::FileName(__FILE__) << ", ln: " << __LINE__ << "]\n"
#define LogWarning(message) Logger.Warning() << message << " [at " << Utility::Files::FileName(__FILE__) << ", ln: " << __LINE__ << "]\n"
#define LogError(message) Logger.Error() << message << " [at " << Utility::Files::FileName(__FILE__) << ", ln: " << __LINE__ << "]\n"
#define LogSuccess(message) Logger.Success() << message << " [at " << Utility::Files::FileName(__FILE__) << ", ln: " << __LINE__ << "]\n"