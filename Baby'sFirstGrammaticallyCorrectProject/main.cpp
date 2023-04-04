#include "Graphics.h"

#include "Application.h"

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include "stb_include.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LOGGER_IMPLEMENTATION
#include "LogMaster.h"

int main()
{
	Logger.Test();

	Application application;

	application.Run();

	glfwTerminate();
	return 0;
}