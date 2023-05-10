#ifdef USE_GRAPHICS
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

class Window
{
public:
	/**
	 * @brief Window is not created, must call init.
	 */
	Window();

	/**
	 * @brief Creates a window under this class instance.
	 * 
	 * @param width - width in screen coordinates of the window
	 * @param height - height in screen coordinates of the window
	 * @param title - The title of the window
	 */
	void init(int width, int height, const char* title);

	/**
	 * @brief Calls cleanup on destruction. Closes window if one is created.
	 */
	~Window();

	/**
	 * @brief Closes the window
	 */
	void cleanup();

	/**
	 * @brief Check if the user is trying to close the window.
	 * 
	 * @return True if the window is still running; false if it should close.
	 */
	bool running();

	/**
	 * @brief Returns the window.
	 * 
	 * Only use for object creation, use this object for all other purposes.
	 * 
	 * @return window handle
	 */
	GLFWwindow* getHandle();

private:
	GLFWwindow* handle;
};
