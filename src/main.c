#include <stdio.h>

#include "window.h"
#include "logging.h"
#include "memory_s.h"

void tick(void) {

}

void render(void) {

}

int main(void) {
	dbgout("program started");
	int ret = window_init(800, 600, "Console");
	if (ret) return ret;
	window_loop(tick, render);
	window_deinit();
	check_memory_leaks();
	dbgout("program finished");
	return 0;
}
