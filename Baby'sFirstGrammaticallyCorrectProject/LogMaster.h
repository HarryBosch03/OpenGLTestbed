#pragma once

#include "FileUtility.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>

typedef std::string(*LogFormatter)(const std::string& in);

const std::string ANSIReset = "\x1b[0;0m";
const std::string ANSIWarning = "\x1b[33;1m";
const std::string ANSIError = "\x1b[41;1m";
const std::string ANSISuccess = "\x1b[32;1m";

const bool PushLogAlways = true;

enum class LogEntryType
{
	None,
	Message,
	Warning,
	Error,
	Success,

	Count,
};

class LogMaster;

struct ILogEntry;
struct LogSeparator;

class LogMaster
{
	std::string append;
	std::stringstream buffer;

public:
	std::ostream* output = &std::cout;

	void Test();

	std::stringstream& Log(const std::string& append = ANSIReset);
	std::stringstream& Log(LogEntryType type, const char* file, int line);

	std::stringstream& Message(const char* file, int line);
	std::stringstream& Warning(const char* file, int line);
	std::stringstream& Error(const char* file, int line);
	std::stringstream& Success(const char* file, int line);
	
	void PushToConsole();

	struct ErrorState
	{
		std::string message;

		ErrorState();
		ErrorState(const ErrorState& other) = delete;
		ErrorState& operator=(const ErrorState& other) = delete;
		~ErrorState();

		void Raise(const std::string& args);
		void Clear();

		inline bool Raised() const { return message.size() > 0; }
	};
};

extern LogMaster Logger;

#define LogMessage(message) Logger.Message(__FILE__, __LINE__) << message
#define LogWarning(message) Logger.Warning(__FILE__, __LINE__) << message
#define LogError(message) Logger.Error(__FILE__, __LINE__) << message
#define LogSuccess(message) Logger.Success(__FILE__, __LINE__) << message
