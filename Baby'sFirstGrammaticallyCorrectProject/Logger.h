#ifndef INCLUDE_LOGGER
#define INCLUDE_LOGGER

#include <string>
#include <windows.h>
#include <iostream>

#define DEF_CALLBACK(n) \
void Set ## n ## Callback(void(*callback)(void*), void* userData = nullptr); \
void Log ## n(const std::string& text);

namespace Logger
{
	void SetCharacterAttributes(int attributes);
	void Init();
	void Log(const std::string& text);
	void Log(const std::string& text, int attributes);

	void PushAttribute(int attrib);
	void PopAttribute();

	DEF_CALLBACK(Warning);
	DEF_CALLBACK(Error);
}

#endif

#ifdef LOGGER_IMPLEMENTATION

HANDLE hConsole;
int defaultAttributes;

#define DEF_CALLBACK_BODY(n, a) \
void(* ## n ## Callback)(void*) = nullptr; \
void* n ## CallbackData = nullptr; \
void Logger::Log ## n (const std::string& text) \
{ \
	Log(text, a); \
	if (n ## Callback) n ## Callback(n ## CallbackData); \
} \
void Logger::Set ## n ## Callback (void(*callback)(void*), void* userData) \
{ \
	n ## Callback = callback; \
	n ## CallbackData = userData; \
}

DEF_CALLBACK_BODY(Warning, FOREGROUND_RED | FOREGROUND_GREEN);
DEF_CALLBACK_BODY(Error, FOREGROUND_RED | FOREGROUND_INTENSITY);

void GetDefaultAttributes()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	defaultAttributes = info.wAttributes;
}

void Logger::Init()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetDefaultAttributes();
}

void Logger::Log(const std::string& text)
{
	std::cout << text;
}

void Logger::Log(const std::string& text, int a)
{
	
	Log(text);
}

void Logger::PushAttribute(int attrib)
{
	GetDefaultAttributes();
	SetConsoleTextAttribute(hConsole, attrib);
}

void Logger::PopAttribute()
{
	SetConsoleTextAttribute(hConsole, defaultAttributes);
}

#endif