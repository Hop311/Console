#include "renderer.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"

#include "tri_vert.h"
#include "tri_frag.h"

#include <GL/glew.h>

static const char *shader_type_name(GLenum shader_type) {
	static const char *vert = "vertex", *geom = "geom", *frag = "frag", *unknown = "unknown";
	switch (shader_type) {
	case GL_VERTEX_SHADER: return vert;
	case GL_GEOMETRY_SHADER: return geom;
	case GL_FRAGMENT_SHADER: return frag;
	default: return unknown;
	}
}
static int load_shader(GLenum shader_type, GLuint *shader, const char *source) {
	assert_s(shader && "[load_shader] shader == NULL");
	assert_s(source && "[load_shader] source == NULL");
	GLint success = GL_FALSE, info_log_length = 0;
	GLuint shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &source, NULL);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		char *const info_log = malloc_s(info_log_length);
		glGetShaderInfoLog(shader_id, info_log_length, NULL, info_log);
		warnout("%s shader info log (length %d):\n%s\n", shader_type_name(shader_type), info_log_length, info_log);
		free_s(info_log);
	}
	if (success) {
		*shader = shader_id;
		return 0;
	} else {
		errout("%s shader compilation failed.\n", shader_type_name(shader_type));
		glDeleteShader(shader_id);
		*shader = 0;
		return -1;
	}
}
static int load_program(GLuint *program, const char *vertex_shader, const char *geometry_shader, const char *fragment_shader) {
	assert_s(program && "[load_shader] program == NULL");
	GLuint vertex_shader_id = 0, geometry_shader_id = 0, fragment_shader_id = 0;
	GLint ret = 0;
	if (vertex_shader) ret |= load_shader(GL_VERTEX_SHADER, &vertex_shader_id, vertex_shader);
	if (geometry_shader) ret |= load_shader(GL_GEOMETRY_SHADER, &geometry_shader_id, geometry_shader);
	if (fragment_shader) ret |= load_shader(GL_FRAGMENT_SHADER, &fragment_shader_id, fragment_shader);
	if (ret) {
		*program = 0;
		return ret;
	}
	GLuint program_id = glCreateProgram();
	if (vertex_shader_id) glAttachShader(program_id, vertex_shader_id);
	if (geometry_shader_id) glAttachShader(program_id, geometry_shader_id);
	if (fragment_shader_id) glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);
	glGetProgramiv(program_id, GL_LINK_STATUS, &ret);
	int info_log_length = 0;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		char *const info_log = malloc_s(info_log_length);
		glGetProgramInfoLog(program_id, info_log_length, NULL, info_log);
		warnout("program info log (length %d):\n%s\n", info_log_length, info_log);
		free_s(info_log);
	}
	glDeleteShader(vertex_shader_id);
	glDeleteShader(geometry_shader_id);
	glDeleteShader(fragment_shader_id);
	if (ret) {
		*program = program_id;
		return 0;
	} else {
		errout("program linking failed.\n");
		glDeleteShader(program_id);
		*program = 0;
		return -1;
	}
}

const float TRI_DATA[] = {
	0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

static GLuint program_tri = 0;
static GLuint vao = 0, vbo = 0;

int renderer_init(void) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		errout("Failed to initialize GLEW\n");
		return -1;
	}

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

	return 0;
}

void renderer_deinit(void) {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program_tri);
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
