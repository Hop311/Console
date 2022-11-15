
cmake_minimum_required(VERSION 3.8)

project("Console")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories("GL/include")
link_directories("GL/lib")

include_directories("src")
set(SRC "src/main.c" "src/window.c" "src/renderer.c" "src/logging.c")

add_executable(Console ${SRC})
target_link_libraries(Console opengl32.lib glfw3dll.lib glew32.lib)

configure_file("GL/bin/glfw3.dll" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("GL/bin/glew32.dll" ${CMAKE_BINARY_DIR} COPYONLY)
