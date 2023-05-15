#include "Window.h"

Window::Window() :
	handle(nullptr)
{
}

void Window::init(int width, int height, const char* title)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	// TODO error handling
}

Window::~Window()
{
	cleanup();
}

void Window::cleanup()
{
	if (handle) {
		glfwDestroyWindow(handle);
		handle = nullptr;
	}
}

bool Window::running()
{
	return !glfwWindowShouldClose(handle);
}

GLFWwindow* Window::getHandle()
{
	return handle;
}