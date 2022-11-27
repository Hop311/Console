#include "window.h"

#include "assert_s.h"
#include "logging.h"
#include "renderer.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <pthread.h>

#include <stdbool.h>
#include <inttypes.h>

#define TARGET_TPS 30
#define TARGET_SPT 1.0/TARGET_TPS

static struct {
	GLFWwindow *glfw_ptr;
	uvec2 dims;
	bool resized;
} window = { 0 };

static struct {
	pthread_t thread;
	pthread_mutex_t close_mutex, resize_mutex;
} loop = {
		.close_mutex = PTHREAD_MUTEX_INITIALIZER,
		.resize_mutex = PTHREAD_MUTEX_INITIALIZER };

static void error_callback(int err, const char *desc) {
	errout("GLFW error %d: %s", err, desc);
}
static void framebuffer_size_callback(GLFWwindow *window_ptr, int width, int height) {
	assert_s(window.glfw_ptr == window_ptr && "[framebuffer_size_callback] window_ptr unrecognised");
	pthread_mutex_lock(&loop.resize_mutex);
	window.dims = (uvec2){{ width, height }};
	window.resized = true;
	pthread_mutex_unlock(&loop.resize_mutex);
}

int window_init(uvec2 dims, const char *title) {
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

	window.glfw_ptr = glfwCreateWindow(dims.width, dims.height, title, NULL, NULL);

	if (window.glfw_ptr == NULL) {
		errout("failed to open GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window.glfw_ptr, framebuffer_size_callback);

	glfwMakeContextCurrent(window.glfw_ptr);

	window.dims = dims;
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
	if (window.glfw_ptr == NULL) {
		errout("window uninitialised");
		return;
	}

	renderer_deinit();

	glfwDestroyWindow(window.glfw_ptr);
	window.glfw_ptr = NULL;
	glfwTerminate();
	dbgout("GLFW cleanup complete");

	pthread_mutex_destroy(&loop.close_mutex);
	pthread_mutex_destroy(&loop.resize_mutex);
}

static void *loop_function(void *args) {
	const window_functions_t *window_functions = args;
	dbgout("loop thread started");
	glfwMakeContextCurrent(window.glfw_ptr);

	double last_second = glfwGetTime(), last_loop = last_second, tick_time_passed = 0.0;
	uint64_t frame_count = 0, tick_count = 0, fps_display = 0, tps_display = 0;

	while (pthread_mutex_trylock(&loop.close_mutex) == 0) {
		const double current_time = glfwGetTime();
		tick_time_passed += current_time - last_loop;

		if (tick_time_passed >= TARGET_SPT) {
			// Tick
			do {
				tick_count++;
				tick_time_passed -= TARGET_SPT;

				if (window.resized) {
					pthread_mutex_lock(&loop.resize_mutex);
					window.resized = false;
					renderer_resize(window.dims, 2.0f);
					pthread_mutex_unlock(&loop.resize_mutex);
				}

				window_functions->tick();
			} while (tick_time_passed >= TARGET_SPT);

			// Frame
			frame_count++;

			window_functions->render(renderer_grid());
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
		pthread_mutex_unlock(&loop.close_mutex);
	}
	glfwMakeContextCurrent(NULL);
	dbgout("finishing loop thread");
	return (void *)0xDEAD;
}

void window_loop(const window_functions_t *window_functions) {
	if (window.glfw_ptr == NULL) {
		errout("window not yet initialised");
		return;
	}
	glfwMakeContextCurrent(NULL);
	dbgout("starting loop thread");
	int ret = pthread_create(&loop.thread, NULL, loop_function, (void *)window_functions);
	if (ret) {
		errout("thread creation failed with code: %d", ret);
		return;
	}

	while (!glfwWindowShouldClose(window.glfw_ptr))
		glfwWaitEvents();

	pthread_mutex_lock(&loop.close_mutex);
	void *thread_ret = NULL;
	pthread_join(loop.thread, &thread_ret);
	pthread_mutex_unlock(&loop.close_mutex);
	dbgout("loop thread finished, returning %p", thread_ret);

	glfwMakeContextCurrent(window.glfw_ptr);
}
