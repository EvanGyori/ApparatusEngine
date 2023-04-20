cmake_minimum_required(VERSION 3.15)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Apparatus VERSION 1.0)

add_library(tester_compiler_flags INTERFACE)
target_compile_features(tester_compiler_flags INTERFACE cxx_std_11)

set(LIBS_LIST)

option(USE_WINDOW "Use Window module" ON)

if (USE_WINDOW)
	add_subdirectory(Window)
	list(APPEND LIBS_LIST Window)
endif()

configure_file(Config.h.in Config.h)

add_executable(Tester tester.cpp)
target_include_directories(Tester PUBLIC
	"${PROJECT_BINARY_DIR}")
target_link_libraries(Tester PUBLIC ${LIBS_LIST} tester_compiler_flags)