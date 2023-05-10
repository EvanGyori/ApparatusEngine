find_package(glfw3 REQUIRED
	NAMES GLFW glfw3)

add_library(Window Window.cpp)
target_include_directories(Window
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Window
	PUBLIC compiler_flags
	PUBLIC glfw)

install(TARGETS Window DESTINATION lib)
install(FILES Window.h DESTINATION include)