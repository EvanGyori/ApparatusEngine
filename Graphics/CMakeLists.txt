if(NOT USE_WINDOW)
add_subdirectory(${PROJECT_SOURCE_DIR}/Window Window)
endif()

find_package(Vulkan REQUIRED)

add_library(Graphics VulkanInstance.cpp DebugMessenger.cpp)
target_include_directories(Graphics
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC Vulkan::Headers)
target_link_libraries(Graphics
	PUBLIC compiler_flags
	PUBLIC Window
	PUBLIC Vulkan::Vulkan)