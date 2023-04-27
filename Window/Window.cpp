#include "Window.h"

GLFWwindow* createWindow()
{

	return glfwCreateWindow(400, 400, "Window", nullptr, nullptr);
}