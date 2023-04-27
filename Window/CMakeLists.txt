find_package(GLFW REQUIRED)

add_library(Window Window.cpp)
target_include_directories(Window
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC ${GLFW_INCLUDE_DIRS})
target_link_libraries(Window
	PUBLIC compiler_flags
	PUBLIC ${GLFW_LIBRARIES})

install(TARGETS Window DESTINATION lib)
install(FILES Window.h DESTINATION include)