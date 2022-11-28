#pragma once

#include "character.h"

#include <stdbool.h>

int renderer_init(void);
void renderer_deinit(void);
void renderer_resize(uvec2 dims, float scale);
void renderer_cursor_pos_update(uvec2 window_pos, bool in_window);
void renderer_render(void);
const character_grid_t *renderer_grid(void);
bool renderer_cursor_in_grid(void);
uvec2 renderer_cursor_pos_grid(void);
