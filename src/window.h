#pragma once

#include "character.h"

typedef struct {
	void (*tick)(void);
	void (*render)(character_grid_t *grid);
} window_functions_t;

int window_init(int width, int height, const char *title);
void window_deinit(void);
void window_loop(const window_functions_t *window_functions);
