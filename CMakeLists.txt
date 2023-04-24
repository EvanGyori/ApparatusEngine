cmake_minimum_required(VERSION 3.15)
# Set the c++ version, replaced by a compile flag
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the project name and version number, central place for version
project(Apparatus VERSION 1.0)

set(LIBS_LIST)

# Add the required c++ version to the compiler flags
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

# Add compiler specific warning flags for debugging
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
	"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
	"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")



# Set the window library as optional, use cmake flag -DUSE_WINDOW=ON/OFF
option(USE_WINDOW "Use Window module" ON)
if(USE_WINDOW)
	add_subdirectory(Window)
	list(APPEND LIBS_LIST Window)
endif()

# Stores variables from cmake into a header file that can be accessed by the program
configure_file(Config.h.in Config.h)

# Create the executable and include the neccessary headers and link libraries
add_executable(Tester tester.cpp)
target_include_directories(Tester PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(Tester PUBLIC ${LIBS_LIST} compiler_flags)

install(TARGETS Tester DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/Config.h DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${Apparatus_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Apparatus_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)