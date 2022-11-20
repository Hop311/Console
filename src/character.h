#pragma once

#include <stdint.h>

typedef struct {
	uint8_t character, colour;
} character_t;

typedef struct {
	size_t width, height, size;
	character_t *chars;
} character_grid_t;

void character_grid_init(character_grid_t *grid, size_t width, size_t height);
void character_grid_free(character_grid_t *grid);
character_t *character_grid_get(const character_grid_t *grid, size_t x, size_t y);

void character_grid_test_fill(character_grid_t *grid);
