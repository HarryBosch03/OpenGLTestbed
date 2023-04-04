#include "LogMaster.h"

#include <vector>

std::vector<LogMaster::ErrorState*> hardErrors;

LogMaster Logger = {};
const std::string Reset = "";

std::string LogMaster::Compile()
{
	PushLast();
	return stream.str();
}

void LogMaster::PushLast()
{
	stream << buffer.str() << Reset;
	buffer = {};
}

void LogMaster::PushToConsole()
{
	std::string compiled = Compile();
	
	std::cout << compiled;
	stream = {};
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

std::stringstream& LogMaster::Message()
{
	PushLast();
	return buffer;
}

std::stringstream& LogMaster::Warning()
{
	PushLast();
	buffer << "\x1B[103;1m";
	return buffer;
}

std::stringstream& LogMaster::Error()
{
	PushLast();
	buffer << "\x1B[101;1m";
	return buffer;
}

std::stringstream& LogMaster::Success()
{
	PushLast();
	buffer << "\x1B[102;1m";
	return buffer;
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