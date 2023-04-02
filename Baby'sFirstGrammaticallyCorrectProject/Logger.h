#pragma once

#include <iostream>
#include <windows.h>
#include <string.h>

#define FILE_NAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define C_ARGS(color, mode) "\033["#color";"#mode

#define C_RESET "\033[0;0m"

#define C_BOLD        1m
#define C_FAINT       2m
#define C_ITALIC      3m
#define C_ULINE       4m
#define C_BLINK       5m
#define C_REVERSE     7m
#define C_INVIS       8m
#define C_STHROUGH    9m

#define C_BLACK       30
#define C_RED         31
#define C_GREEN       32
#define C_YELLOW      33
#define C_BLUE        34
#define C_PURPLE      35
#define C_CYAN        36
#define C_WHITE       37
#define C_DEFAULT_COL 39

#define C_BLACK_BG    40
#define C_RED_BG      41
#define C_GREEN_BG    42
#define C_YELLOW_BG   43
#define C_BLUE_BG     44
#define C_PURPLE_BG   45
#define C_CYAN_BG     46
#define C_WHITE_BG    47
#define C_DEFAULT_BG  49

#define LOG_GENERIC(text, type, color, mode) \
std::cout << C_ARGS(color, mode) << "[" << type << "] " << text << " at (" << FILE_NAME << ": ln." << __LINE__ << ")" << C_RESET << '\n'

#define LOG_ARGS(color, mode, text) LOG_GENERIC(text, "MESSAGE", color, mode)

#define LOG(text) LOG_GENERIC(text, "MESSAGE", 0, 0m)

#define LOG_SUCCESS(text) LOG_GENERIC(text, "SUCCESS", C_GREEN_BG, C_BOLD)

#define LOG_WARNING(text) LOG_GENERIC(text, "WARNING", C_YELLOW_BG, C_BOLD)

#define LOG_ERROR(text) LOG_GENERIC(text, " ERROR ", C_RED_BG, C_BOLD)

#define FNAME(str) str.substr(str.find_last_of("/\\") + 1)

#ifdef _DEBUG

#define LOG_TEST \
std::cout << "---------------------- LOG TEST ----------------------\n"; \
LOG("testing LOG"); \
LOG_ARGS(C_BLUE, C_BLINK, "testing LOG_COLOR"); \
LOG_SUCCESS("testing LOG_SUCCESS"); \
LOG_WARNING("testing LOG_WARNING"); \
LOG_ERROR("testing LOG_ERROR"); \
std::cout << "-------------------- END LOG TEST --------------------\n\n"; \

#else
#define LOG_TEST
#endif