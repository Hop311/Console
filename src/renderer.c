#include "renderer.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"
#include "gltools.h"

#include "lodepng.h"

#include <stdlib.h>

// Shaders
#include "texture_vert.h"
#include "texture_frag.h"

#define CHARSHEET "res/Alloy_curses_12x12_thicker.png"

static const float VERTEX_DATA[] = {
	-0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f, -0.5f, 1.0f, 1.0f
};

static struct {
	GLuint charsheet, program, vao, vbo;
} gl_objects = { 0 };

int renderer_init(void) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		errout("failed to initialize GLEW");
		return -1;
	}

	enable_gl_debug_output();

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	unsigned char *image = NULL;
	unsigned width, height;
	unsigned ret = lodepng_decode32_file(&image, &width, &height, CHARSHEET);
	if (ret) {
		errout("failed to load %s with lodepng error %u: %s", CHARSHEET, ret, lodepng_error_text(ret));
		return -1;
	}
	load_texture(&gl_objects.charsheet, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	free(image);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_objects.charsheet);

	glGenVertexArrays(1, &gl_objects.vao);
	glBindVertexArray(gl_objects.vao);
	glGenBuffers(1, &gl_objects.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl_objects.vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_DATA), VERTEX_DATA, GL_DYNAMIC_DRAW);

	ret = load_program(&gl_objects.program, texture_vert_glsl, NULL, texture_frag_glsl);
	if (ret) {
		errout("failed to load shaders");
		return -1;
	}
	glUseProgram(gl_objects.program);
	glUniform1i(glGetUniformLocation(gl_objects.program, "image"), 0);

	dbgout("OpenGL setup complete");
	return 0;
}

void renderer_deinit(void) {
	glDeleteProgram(gl_objects.program);
	glDeleteVertexArrays(1, &gl_objects.vao);
	glDeleteBuffers(1, &gl_objects.vbo);
	glDeleteTextures(1, &gl_objects.charsheet);

	dbgout("OpenGL cleanup complete");
}

void renderer_resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void renderer_render(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
