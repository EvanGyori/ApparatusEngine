#include <GLFW/glfw3.h>

class Window
{
public:
	Window();

	~Window();
	void cleanup();

	void init(int width, int height, const char* title);

	bool running();

private:
	GLFWwindow* handle;
};