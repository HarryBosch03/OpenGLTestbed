#pragma once

#include "FileUtility.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef std::string(*LogFormatter)(const std::string& in);

enum class LogEntryType
{
	None,
	Message,
	Warning,
	Error,
	Success,

	Count,
};

class LogEntry
{
	std::stringstream stream;
	std::stringstream metadata;
	std::string compiled;
	bool dirty;
	void Metadata(const std::string& file, int line);

public:
	LogEntryType type;

	LogEntry(LogEntryType type);
	LogEntry(LogEntryType type, const std::string& message);

	std::stringstream& Stream();
	const std::stringstream& Stream() const;

	std::stringstream& Metadata();
	const std::stringstream& Metadata() const;
	
	const std::string& Compile();

};

class LogMaster
{
	bool dirty = false;

	std::string Compile();
	std::vector<LogEntry> entries;
	LogEntry& GetNewEntry(LogEntryType type);

public:
	void PushToConsole();
	void Test();

	std::stringstream& Log(LogEntryType type);

	std::stringstream& Message();
	std::stringstream& Warning();
	std::stringstream& Error();
	std::stringstream& Success();
	
	std::stringstream& Log(LogEntryType type, const char* file, int line);

	std::stringstream& Message(const char* file, int line);
	std::stringstream& Warning(const char* file, int line);
	std::stringstream& Error(const char* file, int line);
	std::stringstream& Success(const char* file, int line);

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

#define LogMessage(message) Logger.Message(__FILE__, __LINE__) << message
#define LogWarning(message) Logger.Warning(__FILE__, __LINE__) << message
#define LogError(message) Logger.Error(__FILE__, __LINE__) << message
#define LogSuccess(message) Logger.Success(__FILE__, __LINE__) << message