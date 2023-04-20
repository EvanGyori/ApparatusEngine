#include <iostream>

#include <Config.h>

#ifdef USE_WINDOW
#include "Window.h"
#endif

int main()
{
	auto test = "Hello World\n";
	std::cout << test;
	std::cout << "Version: " << Apparatus_VERSION_MAJOR << "." << Apparatus_VERSION_MINOR << '\n';
	std::cout << "Project directory: " << PROJECT_BINARY_DIR << '\n';
	std::cout << "source directory: " << PROJECT_SOURCE_DIR << '\n';
	#ifdef USE_WINDOW
	std::cout << test() << '\n';
	#endif

	return 0;
}