#include "gltools.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"

static const char *debug_type_name(GLenum type) {
	switch (type) {
#define F(X) case GL_DEBUG_TYPE_##X: return #X;
	F(ERROR) F(DEPRECATED_BEHAVIOR) F(UNDEFINED_BEHAVIOR) F(PORTABILITY)
	F(PERFORMANCE) F(MARKER) F(PUSH_GROUP) F(POP_GROUP) F(OTHER)
#undef F
	default: return "UNKNOWN";
	}
}
static const char *debug_severity_name(GLenum severity) {
	switch (severity) {
#define F(X) case GL_DEBUG_SEVERITY_##X: return #X;
	F(HIGH) F(MEDIUM) F(LOW) F(NOTIFICATION)
#undef F
	default: return "UNKNOWN";
	}
}
void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param) {
	(void)source; (void)id; (void)length; (void)user_param;
	print_log("[%s][%s]\n\n%s", debug_type_name(type), debug_severity_name(severity), message);
}

static const char *shader_type_name(GLenum type) {
	switch (type) {
#define F(X) case GL_##X##_SHADER: return #X;
	F(VERTEX) F(GEOMETRY) F(FRAGMENT)
#undef F
	default: return "UNKNOWN";
	}
}
int load_shader(GLenum shader_type, GLuint *shader, const char *source) {
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
		warnout("%s shader info log (length %d):\n\n%s", shader_type_name(shader_type), info_log_length, info_log);
		free_s(info_log);
	}
	if (success) {
		*shader = shader_id;
		return 0;
	} else {
		errout("%s shader compilation failed", shader_type_name(shader_type));
		glDeleteShader(shader_id);
		*shader = 0;
		return -1;
	}
}
int load_program(GLuint *program, const char *vertex_shader, const char *geometry_shader, const char *fragment_shader) {
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
		warnout("program info log (length %d):\n\n%s", info_log_length, info_log);
		free_s(info_log);
	}
	glDeleteShader(vertex_shader_id);
	glDeleteShader(geometry_shader_id);
	glDeleteShader(fragment_shader_id);
	if (ret) {
		*program = program_id;
		return 0;
	} else {
		errout("program linking failed");
		glDeleteShader(program_id);
		*program = 0;
		return -1;
	}
}
