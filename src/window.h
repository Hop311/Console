#pragma once

#include "character.h"

typedef void (*TickFunction)(void);
typedef void (*RenderFunction)(character_grid_t *grid);

int window_init(int width, int height, const char *title);
void window_deinit(void);
void window_loop(TickFunction tick, RenderFunction render);
