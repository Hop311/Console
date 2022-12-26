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
#define CHAR_PADDING_FACTOR 0.1f

static struct {
	GLuint charsheet, program, vao, vbo;
} gl_objects = { 0 };
static struct {
	uvec2 dims, char_dims;
} window = { 0 };
static struct {
	fvec2 char_dims, top_left;
} screen = { 0 };
static character_grid_t grid = { 0 };

static void set_background_colour(uint8_t colour) {
	const colour_t *bg_colour = &COLOURS[colour & 15];
	glClearColor(bg_colour->r, bg_colour->g, bg_colour->b, 1.0f);
}

int renderer_init(void) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		errout("failed to initialize GLEW");
		return -1;
	}

	enable_gl_debug_output();

	set_background_colour(BLACK);

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
	window.char_dims = (uvec2){{ width / 16, height / 16 }};

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
	glUniform3fv(glGetUniformLocation(gl_objects.program, "colours"), COLOUR_COUNT, COLOURS[0].v);

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

void renderer_resize(uvec2 dims, float scale) {
	assert_s(scale > 0.0f && "[renderer_resize] scale <= 0.0f");
	window.dims = dims;
	glViewport(0, 0, dims.width, dims.height);
	screen.char_dims = (fvec2){{ scale * (float)window.char_dims.width, scale * (float)window.char_dims.height }};
	character_grid_init(&grid, (uvec2) { {
		(uint32_t)((dims.width - screen.char_dims.width * CHAR_PADDING_FACTOR) / screen.char_dims.width),
		(uint32_t)((dims.height - screen.char_dims.height * CHAR_PADDING_FACTOR) / screen.char_dims.height)
	}});
	screen.char_dims.width /= dims.width;
	screen.char_dims.height /= dims.height;
	screen.top_left = (fvec2){{ (float)grid.dims.width * -screen.char_dims.width, (float)grid.dims.height * screen.char_dims.height }};
	screen.char_dims.width *= 2.0f;
	screen.char_dims.height *= 2.0f;

	glBufferData(GL_ARRAY_BUFFER, sizeof(character_t) * grid.size, grid.chars, GL_STREAM_DRAW);
	glUniform1i(glGetUniformLocation(gl_objects.program, "width"), grid.dims.width);
	glUniform2f(glGetUniformLocation(gl_objects.program, "char_dims"), screen.char_dims.width, screen.char_dims.height);
	glUniform2f(glGetUniformLocation(gl_objects.program, "top_left"), screen.top_left.x, screen.top_left.y);
}

void renderer_render(void) {
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(character_t) * grid.size, grid.chars);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, (GLsizei)grid.size);
}

const character_grid_t *renderer_grid(void) {
	return (const character_grid_t *)&grid;
}

fvec2 renderer_window_to_screen_coords(uvec2 window_coords) {
	return (fvec2){{ (float)window_coords.x / (float)window.dims.width * 2.0f - 1.0f,
		(float)window_coords.y / (float)window.dims.height * 2.0f - 1.0f }};
}
uvec2 renderer_screen_to_grid_coords(fvec2 screen_coords) {
	return (uvec2) {{ (uint32_t)((screen_coords.x - screen.top_left.x) / screen.char_dims.width),
		(uint32_t)((float)grid.dims.height - (screen.top_left.y - screen_coords.y) / screen.char_dims.height) }};
}
bool renderer_screen_coords_in_grid(fvec2 screen_coords) {
	return screen_coords.x >= screen.top_left.x && screen_coords.y <= screen.top_left.y &&
		screen_coords.x <= -screen.top_left.x && screen_coords.y >= -screen.top_left.y;
}
