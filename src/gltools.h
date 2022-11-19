#pragma once

#include <GL/glew.h>

void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param);
int load_shader(GLenum shader_type, GLuint *shader, const char *source);
int load_program(GLuint *program, const char *vertex_shader, const char *geometry_shader, const char *fragment_shader);
