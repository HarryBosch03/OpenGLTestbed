#include "LogMaster.h"

#include <vector>
#include <windows.h>
#include <winuser.h>

std::vector<LogMaster::ErrorState*> hardErrors;

LogMaster Logger = {};
const std::string Reset =	"\x1b[0;0m";
const std::string Warning = "\x1b[33;1m";
const std::string Error =	"\x1b[41;1m";
const std::string Success = "\x1b[32;1m";

std::string LogMaster::Compile()
{
	std::stringstream sstream;

	for (LogEntry& entry : entries)
	{
		sstream << entry.Compile() << Reset << '\n';
	}

	return sstream.str();
}

void LogMaster::PushToConsole()
{
	if (!dirty) return;

	std::string compiled = Compile();
	
	std::cout << "\x1b[J" << "\x1b[H";
	std::cout << compiled;

	dirty = false;
}

void LogMaster::Test()
{
#if _DEBUG

	LogMessage("This is a test Message.");
	LogWarning("This is a test Warning.");
	LogError("This is a test Soft Error.");
	LogSuccess("This is a test Success.");

#endif
}

LogEntry& LogMaster::GetNewEntry(LogEntryType type)
{
	LogEntry& entry = entries.emplace_back(type);
	dirty = true;

	if (type == LogEntryType::Error)
	{
		HWND console = GetConsoleWindow();
		FlashWindow(console, true);
	}

	return entry;
}

std::stringstream& LogMaster::Log(LogEntryType type)
{
	return GetNewEntry(type).Stream();
}

std::stringstream& LogMaster::Message() { return Log(LogEntryType::Message); }
std::stringstream& LogMaster::Warning() { return Log(LogEntryType::Warning); }
std::stringstream& LogMaster::Error() { return Log(LogEntryType::Error); }
std::stringstream& LogMaster::Success() { return Log(LogEntryType::Success); }

std::stringstream& LogMaster::Log(LogEntryType type, const char* file, int line)
{
	LogEntry& entry = GetNewEntry(type);
	entry.Metadata() << " [at " << Utility::Files::FileName(__FILE__) << ", ln: " << __LINE__ << "]";
	return entry.Stream();
}

std::stringstream& LogMaster::Message(const char* file, int line) { return Log(LogEntryType::Message, file, line); }
std::stringstream& LogMaster::Warning(const char* file, int line) { return Log(LogEntryType::Warning, file, line); }
std::stringstream& LogMaster::Error(const char* file, int line) { return Log(LogEntryType::Error, file, line); }
std::stringstream& LogMaster::Success(const char* file, int line) { return Log(LogEntryType::Success, file, line); }

LogMaster::ErrorState::ErrorState()
{
	hardErrors.push_back(this);
}

LogMaster::ErrorState::~ErrorState()
{
	for (auto it = hardErrors.begin(); it != hardErrors.end(); ++it)
	{
		if (*it != this) continue;
		hardErrors.erase(it);
		break;
	}
}

void LogMaster::ErrorState::Raise(const std::string& args)
{
	message = args;
}

void LogMaster::ErrorState::Clear()
{
	message = "";
}

LogEntry::LogEntry(LogEntryType type) : type(type) 
{
	switch (type)
	{
	case LogEntryType::None:
		break;
	case LogEntryType::Message:
		Stream() << "[MESSAGE]  ";
		break;
	case LogEntryType::Warning:
		Stream() << Warning << "[WARNING]  ";
		break;
	case LogEntryType::Error:
		Stream() << Error << "[ ERROR ]  ";
		break;
	case LogEntryType::Success:
		Stream() << Success << "[SUCCESS]  ";
		break;
	case LogEntryType::Count:
		LogError("\"LogEntryType::Count\" is not a valid Log Entry Type");
		break;
	}
}
LogEntry::LogEntry(LogEntryType type, const std::string& message) : LogEntry(type)
{
	Stream() << message;
}

const std::stringstream& LogEntry::Stream() const { return stream; }
std::stringstream& LogEntry::Stream()
{
	dirty = true;
	return stream;
}

const std::stringstream& LogEntry::Metadata() const { return metadata; }
std::stringstream& LogEntry::Metadata()
{
	dirty = true;
	return metadata;
}

const std::string& LogEntry::Compile()
{
	if (dirty)
	{
		compiled = stream.str();
		dirty = false;
	}
	return compiled;
}