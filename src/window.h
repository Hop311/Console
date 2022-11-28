#pragma once

#include "character.h"

typedef struct {
	void (*tick)(void);
	void (*render)(void);
} window_functions_t;

int window_init(uvec2 dims, const char *title);
void window_deinit(void);
void window_loop(const window_functions_t *window_functions);
