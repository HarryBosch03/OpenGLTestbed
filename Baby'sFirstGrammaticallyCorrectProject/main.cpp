#include "Graphics.h"

#include "Application.h"

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_NONE
#include "stb_include.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LOGGER_IMPLEMENTATION
#include "Logger.h"

int main()
{
	LOG_TEST

	Application application;

	application.Run();

	glfwTerminate();
	return 0;
}