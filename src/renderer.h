#pragma once

#include "character.h"

int renderer_init(void);
void renderer_deinit(void);
void renderer_resize(uvec2 dims, float scale);
void renderer_render(void);
character_grid_t *renderer_grid(void);
