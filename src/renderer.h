#pragma once

#include "character.h"

#include <stdbool.h>

int renderer_init(void);
void renderer_deinit(void);
void renderer_resize(uvec2 dims, float scale);
void renderer_render(void);

const character_grid_t *renderer_grid(void);
void renderer_cursor_pos_update(ivec2 pos_window, bool in_window, uvec2 *pos_grid, bool *in_grid);
