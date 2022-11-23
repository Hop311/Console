#include "renderer.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"
#include "gltools.h"
#include "colour.h"

#include "lodepng.h"

#include <stdlib.h>

// Shaders
#include "char_vert.h"
#include "char_geom.h"
#include "char_frag.h"

#define CHARSHEET "res/Alloy_curses_12x12_thicker.png"
#define CHAR_PADDING_FACTOR 0.5f

static struct {
	GLuint charsheet, program, vao, vbo;
} gl_objects = { 0 };
static struct {
	uint32_t width, height;
} character;

character_grid_t grid = { 0 };

static void set_background_colour(uint8_t colour) {
	colour &= 15;
	glClearColor(COLOURS[colour][0], COLOURS[colour][1], COLOURS[colour][2], 1.0f);
}

int renderer_init(void) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		errout("failed to initialize GLEW");
		return -1;
	}

	enable_gl_debug_output();

	set_background_colour(LIGHT_BLUE);

	unsigned char *image = NULL;
	unsigned width, height;
	int ret = lodepng_decode_file(&image, &width, &height, CHARSHEET, LCT_PALETTE, 8);
	if (ret) {
		errout("failed to load %s with lodepng error %u: %s", CHARSHEET, ret, lodepng_error_text(ret));
		return -1;
	}
	if (width == 0 || height == 0 || width & 15 || height & 15) {
		errout("invalid charsheet dimensions %ux%u (both must be non-zero multiples of 16)", width, height);
		return -1;
	}
	character.width = width / 16;
	character.height = height / 16;

	load_texture(&gl_objects.charsheet, GL_R8, width, height, GL_RED, GL_UNSIGNED_BYTE, image);
	free(image);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_objects.charsheet);

	glGenVertexArrays(1, &gl_objects.vao);
	glBindVertexArray(gl_objects.vao);
	glGenBuffers(1, &gl_objects.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl_objects.vbo);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_SHORT, sizeof(character_t), NULL);
	glEnableVertexAttribArray(0);

	ret = load_program(&gl_objects.program, char_vert_glsl, char_geom_glsl, char_frag_glsl);
	if (ret) {
		errout("failed to load shaders");
		return -1;
	}
	glUseProgram(gl_objects.program);
	glUniform1i(glGetUniformLocation(gl_objects.program, "charsheet"), 0);
	glUniform3fv(glGetUniformLocation(gl_objects.program, "colours"), sizeof(COLOURS)/3, &COLOURS[0][0]);

	dbgout("OpenGL setup complete");
	return 0;
}

void renderer_deinit(void) {
	character_grid_free(&grid);

	glDeleteProgram(gl_objects.program);
	glDeleteVertexArrays(1, &gl_objects.vao);
	glDeleteBuffers(1, &gl_objects.vbo);
	glDeleteTextures(1, &gl_objects.charsheet);

	dbgout("OpenGL cleanup complete");
}

void renderer_resize(int width, int height, float scale) {
	glViewport(0, 0, width, height);
	float scaled_char_width = scale * (float)character.width;
	float scaled_char_height = scale * (float)character.height;
	const uint32_t w = (uint32_t)((width - scaled_char_width * CHAR_PADDING_FACTOR) / scaled_char_width);
	const uint32_t h = (uint32_t)((height - scaled_char_height * CHAR_PADDING_FACTOR) / scaled_char_height);
	scaled_char_width /= width;
	scaled_char_height /= height;
	character_grid_init(&grid, w, h);

	glBufferData(GL_ARRAY_BUFFER, sizeof(character_t) * grid.size, grid.chars, GL_STREAM_DRAW);
	glUniform1i(glGetUniformLocation(gl_objects.program, "width"), (GLuint)grid.width);
	glUniform2f(glGetUniformLocation(gl_objects.program, "char_dims"), scaled_char_width * 2.0f, scaled_char_height * 2.0f);
	glUniform2f(glGetUniformLocation(gl_objects.program, "top_left"),
		(float)w * -scaled_char_width, (float)h * scaled_char_height);
}

void renderer_render(void) {
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(character_t) * grid.size, grid.chars);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, (GLsizei)grid.size);
}

character_grid_t *renderer_grid(void) {
	return &grid;
}
