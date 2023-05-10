#include <iostream>

#include <Config.h>

#ifdef USE_WINDOW
#include "Window.h"
#endif

#ifdef USE_GRAPHICS
#include "VulkanInstance.h"
#include "DebugMessenger.h"
#endif

int main()
{
	auto hello = "Hello World\n";
	std::cout << hello;
	std::cout << "Version: " << Apparatus_VERSION_MAJOR << "." << Apparatus_VERSION_MINOR << '\n';
	#ifdef USE_WINDOW
	glfwInit();
	
	Window window;
	window.init(500, 500, "tester");
	while (window.running()) {
		glfwPollEvents();
	}
	window.cleanup();

	glfwTerminate();
	#endif

	#ifdef USE_GRAPHICS
	try {
		VulkanInstance instance;
		instance.init("Test");
		DebugMessenger debugMessenger;
		debugMessenger.init(instance);
		debugMessenger.cleanup();
		instance.cleanup();
	} catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	#endif

	return 0;
}