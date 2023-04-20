add_library(Window Window.cpp)
target_include_directories(Window INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Window tester_compiler_flags)