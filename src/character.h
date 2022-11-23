#pragma once

#include <stdint.h>

typedef struct {
	uint8_t character, colour;
} character_t;

typedef struct {
	uint32_t width, height, size;
	character_t *chars;
} character_grid_t;

void character_grid_init(character_grid_t *grid, uint32_t width, uint32_t height);
void character_grid_free(character_grid_t *grid);
character_t *character_grid_get(const character_grid_t *grid, uint32_t x, uint32_t y);
