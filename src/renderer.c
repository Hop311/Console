#include "renderer.h"

#include "logging.h"

#include <GL/glew.h>

int renderer_init(int width, int height) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		errout("Failed to initialize GLEW\n");
		return -1;
	}

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	
	renderer_resize(width, height);

	return 0;
}

void renderer_resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void renderer_render(void) {
	glClear(GL_COLOR_BUFFER_BIT);
}
