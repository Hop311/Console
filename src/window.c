#include "window.h"

#include "assert_s.h"
#include "logging.h"
#include "renderer.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdbool.h>

#define TARGET_TPS 30
#define TARGET_SPT 1.0/TARGET_TPS

static GLFWwindow *window = NULL;
static int window_width = 0, window_height = 0;
static bool resized = false;

static void error_callback(int err, const char *desc) {
	errout("GLFW error %d: %s\n", err, desc);
}
static void framebuffer_size_callback(GLFWwindow *_window, int width, int height) {
	assert_s(window == _window && "[framebuffer_size_callback] _window unrecognised");
	window_width = width;
	window_height = height;
	resized = true;
}

int window_init(int width, int height, const char *title) {
	if (window) {
		errout("Window already created.\n");
		return -1;
	}
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		errout("Failed to initialise GLFW.\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		errout("Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (renderer_init()) {
		errout("Failed to initialize render engine.\n");
		glfwTerminate();
		return -1;
	}

	window_width = width;
	window_height = height;
	resized = true;

	return 0;
}

void window_deinit(void) {
	renderer_deinit();
	glfwDestroyWindow(window);
	window = NULL;
	glfwTerminate();
}

void window_loop(TickFunction tick, RenderFunction render) {
	if (!window) {
		errout("window not yet initialised.\n");
		return;
	}

	double last_second = glfwGetTime(), last_loop = last_second, tick_time_passed = 0.0;
	size_t frame_count = 0, tick_count = 0, fps_display = 0, tps_display = 0;
	
	while (!glfwWindowShouldClose(window)) {
		const double current_time = glfwGetTime();
		tick_time_passed += current_time - last_loop;

		if (tick_time_passed >= TARGET_SPT) {
			// Tick
			while (tick_time_passed >= TARGET_SPT) {
				tick_count++;
				double polling_time = glfwGetTime();
				glfwPollEvents();
				polling_time = glfwGetTime() - polling_time;
				tick_time_passed -= TARGET_SPT + polling_time;

				if (resized) {
					resized = false;
					renderer_resize(window_width, window_height);
				}

				tick();
			}

			// Frame
			frame_count++;

			render();
			renderer_render();

			glfwSwapBuffers(window);
		}

		// Trigger each second
		if (current_time - last_second >= 1.0) {
			last_second = current_time;
			fps_display = frame_count;
			tps_display = tick_count;
			frame_count = 0;
			tick_count = 0;
			dbgout("FPS: %zu, TPS: %zu\n", fps_display, tps_display);
		}
		last_loop = current_time;
	}
}
