#include "Graphics.h"

#include "Application.h"

int main()
{
	Application application;

	application.Run();

	glfwTerminate();
	return 0;
}