#include "Window.h"

Window::Window() :
	handle(nullptr)
{
}

Window::~Window()
{
	cleanup();
}

void Window::cleanup()
{
	if (handle) {
		glfwDestroyWindow(handle);
	}
}

void Window::init(int width, int height, const char* title)
{
	handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	// TODO error handling
}

bool Window::running()
{
	return !glfwWindowShouldClose(handle);
}