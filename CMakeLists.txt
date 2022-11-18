
cmake_minimum_required(VERSION 3.15)

project("Console")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
endif()

# Add dependency folders
include_directories("GL/include")
link_directories("GL/lib")

# Add C headers and sources
include_directories("src")
set(SRC "src/main.c" "src/window.c" "src/renderer.c" "src/logging.c" "src/memory_s.c" "src/gltools.c")

# Generate shader headers
include_directories("shader/include")
set(SHADERS "tri_vert" "tri_frag")
foreach(SHADER ${SHADERS})
    set(INPUT_FILE "${SHADER}.glsl")
    set(OUTPUT_FILE "include/${SHADER}.h")
    message("Sending ${INPUT_FILE} to ${OUTPUT_FILE}")
    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        DEPENDS "shader/${INPUT_FILE}"
        COMMAND cd "${CMAKE_SOURCE_DIR}/shader"
        COMMAND ${CMAKE_COMMAND} -E make_directory "include"
        COMMAND xxd -i ${INPUT_FILE} ${OUTPUT_FILE}
        # Make declarations static const
        COMMAND sed -i "s/unsigned/\\nstatic const/" ${OUTPUT_FILE}
        # Add terminating null character to array
        COMMAND sed -i "s/\\([0-9a-f]\\)$$/&, 0x00/" ${OUTPUT_FILE}
        # Add shader file title
        COMMAND sed -i "1s/^/\\/\\/ ${INPUT_FILE}\\n/" ${OUTPUT_FILE}
        COMMENT "Compiling ${INPUT_FILE} to binary")
    LIST(APPEND SHADERS_INCLUDE ${OUTPUT_FILE})
endforeach()

# Create exe and link libraries
add_executable(Console ${SRC} ${SHADERS_INCLUDE})
target_link_libraries(Console opengl32.lib glfw3dll.lib glew32.lib)

# Copy dlls to build directory
configure_file("GL/bin/glfw3.dll" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("GL/bin/glew32.dll" ${CMAKE_BINARY_DIR} COPYONLY)

# Clean shader header folder
set_property(
    TARGET Console
    APPEND
    PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_SOURCE_DIR}/shader/include")
