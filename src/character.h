#pragma once

#include "vec.h"

typedef struct {
	uint8_t character, colour;
} character_t;

typedef struct {
	uvec2 dims;
	uint32_t size;
	character_t *chars;
} character_grid_t;

void character_grid_init(character_grid_t *grid, uvec2 dims);
void character_grid_free(character_grid_t *grid);
character_t *character_grid_get(const character_grid_t *grid, uvec2 point);
