#pragma once

#include "character.h"

#include <stdbool.h>

typedef struct {
	int key, action, mods;
} key_event_t;

typedef struct {
	int button, action, mods;
	ivec2 pos;
	uvec2 dims;
} mousebutton_event_t;

extern const int ACTION_PRESS;
extern const int KEY_ESCAPE;
extern const int MOUSEBUTTON_LEFT;

typedef struct {
	void (*tick)(const key_event_t *key_events, const mousebutton_event_t *mousebutton_events);
	void (*render)(void);
} window_functions_t;

int window_init(uvec2 dims, const char *title);
void window_deinit(void);
void window_loop(const window_functions_t *window_functions);

void window_close(void);
bool window_cursor_in_grid(void);
uvec2 window_cursor_pos_grid(void);
