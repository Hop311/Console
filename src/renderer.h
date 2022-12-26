#pragma once

#include "character.h"

#include <stdbool.h>

int renderer_init(void);
void renderer_deinit(void);
void renderer_resize(uvec2 dims, float scale);
void renderer_render(void);

const character_grid_t *renderer_grid(void);

fvec2 renderer_window_to_screen_coords(uvec2 window_coords);
uvec2 renderer_screen_to_grid_coords(fvec2 screen_coords);
bool renderer_screen_coords_in_grid(fvec2 screen_coords);
