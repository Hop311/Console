
cmake_minimum_required(VERSION 3.15)

project("Console")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

if(MSVC)
	add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
	add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
endif()

# Add sources
set(SRC "src/main.c" "src/window.c" "src/renderer.c" "src/logging.c" "src/memory_s.c" "src/gltools.c" "src/character.c" "src/colour.c")

# Generate shader headers
include_directories("shader/include")
set(SHADERS "char_vert" "char_geom" "char_frag")
foreach(SHADER ${SHADERS})
	set(INPUT_FILE "${SHADER}.glsl")
	set(OUTPUT_FILE "${SHADER}.h")
	set(OUTPUT_FILEPATH "${CMAKE_SOURCE_DIR}/shader/include/${OUTPUT_FILE}")
	message("Sending ${INPUT_FILE} to ${OUTPUT_FILE}")
	add_custom_command(
		OUTPUT ${OUTPUT_FILEPATH} DEPENDS "shader/${INPUT_FILE}"
		COMMAND cd "${CMAKE_SOURCE_DIR}/shader"
		COMMAND ${CMAKE_COMMAND} -E make_directory "include"
		COMMAND xxd -i ${INPUT_FILE} "include/${OUTPUT_FILE}"
		COMMAND cd "include"
		# Make declarations static const
		COMMAND sed -i "s/unsigned/\\nstatic const/" ${OUTPUT_FILE}
		# Add terminating null character to array
		COMMAND sed -i "s/\\([0-9a-f]\\)$$/&, 0x00/" ${OUTPUT_FILE}
		# Add shader file title
		COMMAND sed -i "1s/^/\\/\\/ ${INPUT_FILE}\\n/" ${OUTPUT_FILE}
		COMMENT "Compiling ${INPUT_FILE} to binary")
	LIST(APPEND SHADERS_INCLUDE ${OUTPUT_FILEPATH})
endforeach()

# Create exe
add_executable(Console ${SRC} ${SHADERS_INCLUDE})

# Add dependencies
target_include_directories(Console PRIVATE "dependencies/include" "dependencies/lodepng")
target_link_directories(Console PRIVATE "dependencies/lib")
target_link_libraries(Console opengl32.lib glfw3dll.lib glew32.lib)
target_sources(Console PRIVATE "dependencies/lodepng/lodepng.c")

# Copy dlls to build directory
set(DYNAMIC_LIBS "glfw3.dll" "glew32.dll")
foreach(LIB ${DYNAMIC_LIBS})
	message("Copying ${LIB} to build directory")
	add_custom_command(
		OUTPUT ${LIB} DEPENDS "dependencies/bin/${LIB}"
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/dependencies/bin/${LIB}" .
		COMMENT "Copying ${LIB} to build directory")
endforeach()
target_sources(Console PRIVATE ${DYNAMIC_LIBS})

# Copy resources to build directory
set(RESOURCES "Alloy_curses_12x12_thicker.png")
foreach(RES ${RESOURCES})
	set(RES_FILEPATH "res/${RES}")
	set(RES_FILEPATH_OUT "${CMAKE_BINARY_DIR}/${RES_FILEPATH}")
	message("Copying ${RES} to build directory res folder")
	add_custom_command(
		OUTPUT ${RES_FILEPATH_OUT} DEPENDS ${RES_FILEPATH}
		COMMAND ${CMAKE_COMMAND} -E make_directory "res"
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${RES_FILEPATH}" "res"
		COMMENT "Copying ${RES} to build directory res folder")
	LIST(APPEND RESOURCES_INCLUDE ${RES_FILEPATH_OUT})
endforeach()
target_sources(Console PRIVATE ${RESOURCES_INCLUDE})

# Extra clean instructions
set_property( TARGET Console APPEND PROPERTY ADDITIONAL_CLEAN_FILES
	"${CMAKE_SOURCE_DIR}/shader/include" "res" DYNAMIC_LIBS)
