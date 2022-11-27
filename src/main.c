#include <stdio.h>

#include "window.h"
#include "logging.h"
#include "assert_s.h"
#include "memory_s.h"
#include "colour.h"

const uvec2 INITIAL_DIMS = {{ 800, 600 }};

int offset = 0;

void tick(void) {
	offset++;
}

void render(character_grid_t *grid) {
	assert_s(grid && "[render] grid == NULL");
	for (uint32_t i = 0; i < grid->size; ++i)
		grid->chars[i] = (character_t){ .character = '0' + (i + offset / 3) % 10, .colour = pack_colours(((i*(i+1) + offset*offset) / 5) & 1 ? GREEN : LIGHT_GREEN, BLACK) };
}

int main(void) {
	dbgout("program started");
	int ret = window_init(INITIAL_DIMS, "Console");
	if (ret) return ret;
	const window_functions_t window_functions = { tick, render };
	window_loop(&window_functions);
	window_deinit();
	check_memory_leaks();
	dbgout("program finished");
	return 0;
}
