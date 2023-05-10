cmake_minimum_required(VERSION 3.15)
project(Apparatus VERSION 1.0)

set(LIBS_LIST)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
	"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
	"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")

option(USE_WINDOW "Use window module" ON)
if(USE_WINDOW)
	add_subdirectory(Window)
	list(APPEND LIBS_LIST Window)
endif()

option(USE_GRAPHICS "Use Graphics module; Window is included" ON)
if(USE_GRAPHICS)
	add_subdirectory(Graphics)
	list(APPEND LIBS_LIST Graphics)
endif()

# remove
find_package(Vulkan REQUIRED)

configure_file(Config.h.in Config.h)

add_executable(Tester tester.cpp)
target_include_directories(Tester PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(Tester ${LIBS_LIST} compiler_flags)

install(TARGETS Tester DESTINATION bin)
install(FILES {PROJECT_BINARY_DIR}/Config.h DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${Apparatus_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Apparatus_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)