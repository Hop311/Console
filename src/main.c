#include <stdio.h>

#include "window.h"
#include "logging.h"
#include "assert_s.h"
#include "memory_s.h"
#include "colour.h"
#include "renderer.h"

static const uvec2 INITIAL_DIMS = {{ 800, 600 }};

static struct {
	bool in_grid;
	uvec2 pos;
	uvec2 *drawn;
} cursor = { 0 };

void tick(const input_event_t *key_events, const input_event_t *mousebutton_events) {

	for_buf(i, key_events) {
		input_event_t ke = key_events[i];
		dbgout("key event: %x/%x/%x", ke.key, ke.action, ke.mods);
		if (ke.key == KEY_ESCAPE && ke.action == ACTION_PRESS) window_close();
	}

	static bool pressed = false;

	for_buf(i, mousebutton_events) {
		input_event_t me = mousebutton_events[i];
		dbgout("mouse event: %x/%x/%x", me.key, me.action, me.mods);
		if (me.key == MOUSEBUTTON_LEFT) pressed = me.action == ACTION_PRESS;
	}

	cursor.in_grid = window_cursor_in_grid();
	if (cursor.in_grid) {
		cursor.pos = window_cursor_pos_grid();
		if (pressed) buf_push(cursor.drawn, cursor.pos);
	}
}

void render(void) {
	const character_grid_t *grid = renderer_grid();
	assert_s(grid && "[render] grid == NULL");
	character_grid_clear(grid);
	for_buf(i, cursor.drawn) {
		character_t *c = character_grid_get(grid, cursor.drawn[i]);
		if (c != NULL) {
			c->character++;
			c->colour = colour_pack(LIGHT_GREEN, WHITE);
		}
	}
	if (cursor.in_grid) {
		character_t *c = character_grid_get(grid, cursor.pos);
		if (c == NULL) errout("invalid cursor position: (%u, %u)", cursor.pos.x, cursor.pos.y);
		else c->colour = colour_pack(colour_foreground(c->colour), BROWN);
	}
}

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);

	dbgout("program started");
	int ret = window_init(INITIAL_DIMS, "Console");
	if (ret) return ret;
	const window_functions_t window_functions = { tick, render };
	window_loop(&window_functions);
	window_deinit();

	buf_free(cursor.drawn);

	check_memory_leaks();
	dbgout("program finished");
	return 0;
}
