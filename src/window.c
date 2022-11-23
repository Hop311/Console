#include "window.h"

#include "assert_s.h"
#include "logging.h"
#include "renderer.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <inttypes.h>

#define TARGET_TPS 30
#define TARGET_SPT 1.0/TARGET_TPS

static struct {
	GLFWwindow *glfw_ptr;
	int width, height;
	bool resized;
} window = { 0 };

static void error_callback(int err, const char *desc) {
	errout("GLFW error %d: %s", err, desc);
}
static void framebuffer_size_callback(GLFWwindow *window_ptr, int width, int height) {
	assert_s(window.glfw_ptr == window_ptr && "[framebuffer_size_callback] window_ptr unrecognised");
	window.width = width;
	window.height = height;
	window.resized = true;
}

int window_init(int width, int height, const char *title) {
	if (window.glfw_ptr) {
		errout("window already created");
		return -1;
	}
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		errout("failed to initialise GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window.glfw_ptr = glfwCreateWindow(width, height, title, NULL, NULL);

	if (window.glfw_ptr == NULL) {
		errout("failed to open GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window.glfw_ptr);
	glfwSetFramebufferSizeCallback(window.glfw_ptr, framebuffer_size_callback);

	window.width = width;
	window.height = height;
	window.resized = true;

	dbgout("GLFW setup complete");

	if (renderer_init()) {
		errout("failed to initialize render engine");
		glfwTerminate();
		return -1;
	}

	return 0;
}

void window_deinit(void) {
	renderer_deinit();
	glfwDestroyWindow(window.glfw_ptr);
	window.glfw_ptr = NULL;
	glfwTerminate();

	dbgout("GLFW cleanup complete");
}

void window_loop(TickFunction tick, RenderFunction render) {
	if (window.glfw_ptr == NULL) {
		errout("window not yet initialised");
		return;
	}

	double last_second = glfwGetTime(), last_loop = last_second, tick_time_passed = 0.0;
	uint64_t frame_count = 0, tick_count = 0, fps_display = 0, tps_display = 0;

	while (!glfwWindowShouldClose(window.glfw_ptr)) {
		const double current_time = glfwGetTime();
		tick_time_passed += current_time - last_loop;

		if (tick_time_passed >= TARGET_SPT) {
			// Tick
			do {
				tick_count++;
				double polling_time = glfwGetTime();
				glfwPollEvents();
				polling_time = glfwGetTime() - polling_time;
				tick_time_passed -= TARGET_SPT + polling_time;

				if (window.resized) {
					window.resized = false;
					renderer_resize(window.width, window.height, 2.0f);
					dbgout("resized window to %dx%d", window.width, window.height);
				}

				tick();
			} while (tick_time_passed >= TARGET_SPT);

			// Frame
			frame_count++;

			render(renderer_grid());
			renderer_render();

			glfwSwapBuffers(window.glfw_ptr);
		}

		// Trigger each second
		if (current_time - last_second >= 1.0) {
			last_second = current_time;
			fps_display = frame_count;
			tps_display = tick_count;
			frame_count = 0;
			tick_count = 0;
			if (fps_display != TARGET_TPS || tps_display != TARGET_TPS)
				dbgout("FPS: %"PRIu64", TPS: %"PRIu64, fps_display, tps_display);
		}
		last_loop = current_time;
	}
}
