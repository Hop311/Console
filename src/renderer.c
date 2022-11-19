#include "renderer.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"
#include "gltools.h"

// Shaders
#include "tri_vert.h"
#include "tri_frag.h"

static const float TRI_DATA[] = {
	0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

static GLuint program_tri = 0;
static GLuint vao = 0, vbo = 0;

int renderer_init(void) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		errout("failed to initialize GLEW");
		return -1;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, 0);

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	int ret = load_program(&program_tri, tri_vert_glsl, NULL, tri_frag_glsl);
	if (ret) return ret;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(TRI_DATA), TRI_DATA, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	dbgout("OpenGL setup complete");
	return 0;
}

void renderer_deinit(void) {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program_tri);

	dbgout("OpenGL cleanup complete");
}

void renderer_resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void renderer_render(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program_tri);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
