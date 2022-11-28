#include <stdio.h>

#include "window.h"
#include "logging.h"
#include "assert_s.h"
#include "memory_s.h"
#include "colour.h"
#include "renderer.h"

const uvec2 INITIAL_DIMS = {{ 800, 600 }};

int offset = 0;

void tick(void) {
	offset++;
}

void render(void) {
	const character_grid_t *grid = renderer_grid();
	assert_s(grid && "[render] grid == NULL");
	for (uint32_t i = 0; i < grid->size; ++i)
		grid->chars[i] = (character_t){ .character = '0' + (i + offset / 3) % 10, .colour = colour_pack(((i*(i+1) + offset*offset) / 5) & 1 ? GREEN : LIGHT_GREEN, BLACK) };
	if (renderer_cursor_in_grid()) {
		character_t *c = character_grid_get(grid, renderer_cursor_pos_grid());
		if (c == NULL) errout("invalid cursor position: (%u, %u)", renderer_cursor_pos_grid().x, renderer_cursor_pos_grid().y);
		else c->colour = colour_invert(c->colour);
	}
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
