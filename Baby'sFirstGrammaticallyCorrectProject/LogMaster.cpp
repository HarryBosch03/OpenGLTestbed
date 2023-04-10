#include "LogMaster.h"

#include <vector>
#include <iostream>
#include <string>

std::vector<LogMaster::ErrorState*> hardErrors;

LogMaster Logger = {};

int lastConsoleRows, lastConsoleColumns;

void GetConsoleSize(int* rows, int* columns)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	if (columns) * columns = info.srWindow.Right - info.srWindow.Left + 1;
	if (rows) *rows = info.srWindow.Bottom - info.srWindow.Top + 1;
}

void LogMaster::Test()
{
#if _DEBUG

	Log() << "\n-------------[TEST]-------------\n";
	LogMessage("This is a test Message.");
	LogWarning("This is a test Warning.");
	LogError("This is a test Soft Error.");
	LogSuccess("This is a test Success.");
	Log() << "-----------[END TEST]-----------\n\n";

#endif
}

std::stringstream& LogMaster::Log(const std::string& append)
{
	buffer << this->append;
	this->append = append;
	return buffer;
}

std::stringstream& LogMaster::Log(LogEntryType type, const char* file, int line)
{
	std::stringstream& buffer = Log(std::string(" at file \"") + Utility::Files::FileName(file) + "\" ln: " + std::to_string(line) + ANSIReset + "\n");

	switch (type)
	{
	case LogEntryType::None:
		break;
	case LogEntryType::Message:
		buffer << ANSIReset << "[MESSAGE]  ";
		break;
	case LogEntryType::Warning:
		buffer << ANSIWarning << "[WARNING]  ";
		break;
	case LogEntryType::Error:
		buffer << ANSIError << "[ ERROR ]  ";
		break;
	case LogEntryType::Success:
		buffer << ANSISuccess << "[SUCCESS]  ";
		break;
	case LogEntryType::Count:
		LogError("\"LogEntryType::Count\" is not a valid Log Entry Type");
		break;
	}

	return buffer;
}

std::stringstream& LogMaster::Message(const char* file, int line) { return Log(LogEntryType::Message, file, line); }
std::stringstream& LogMaster::Warning(const char* file, int line) { return Log(LogEntryType::Warning, file, line); }
std::stringstream& LogMaster::Error(const char* file, int line) { return Log(LogEntryType::Error, file, line); }
std::stringstream& LogMaster::Success(const char* file, int line) { return Log(LogEntryType::Success, file, line); }

void LogMaster::PushToConsole()
{
	*output << Log().str();
	buffer = {};
}

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