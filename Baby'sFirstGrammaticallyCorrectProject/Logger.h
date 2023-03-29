#pragma once

#include <iostream>
#include <windows.h>

#define CONSOLE_COLOR(col, bold) \
"\033["#bold";"#col"m"

#define LOG_COLOR(col, text) \
std::cout << col << text << C_DEFAULT;

#define C_BLACK  CONSOLE_COLOR(30, 0)
#define C_RED    CONSOLE_COLOR(31, 0)
#define C_GREEN  CONSOLE_COLOR(32, 0)
#define C_YELLOW CONSOLE_COLOR(33, 0)
#define C_BLUE   CONSOLE_COLOR(34, 0)
#define C_PURPLE CONSOLE_COLOR(35, 0)
#define C_CYAN   CONSOLE_COLOR(36, 0)
#define C_WHITE  CONSOLE_COLOR(37, 0)

#define C_BLACK_B  CONSOLE_COLOR(30, 1)
#define C_RED_B    CONSOLE_COLOR(31, 1)
#define C_GREEN_B  CONSOLE_COLOR(32, 1)
#define C_YELLOW_B CONSOLE_COLOR(33, 1)
#define C_BLUE_B   CONSOLE_COLOR(34, 1)
#define C_PURPLE_B CONSOLE_COLOR(35, 1)
#define C_CYAN_B   CONSOLE_COLOR(36, 1)
#define C_WHITE_B  CONSOLE_COLOR(37, 1)

#define C_DEFAULT C_WHITE