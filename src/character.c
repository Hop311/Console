#include "character.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"

void character_grid_init(character_grid_t *grid, size_t width, size_t height) {
	assert_s(grid && "[character_grid_init] grid == NULL");
	character_grid_free(grid);
	if (width == 0 || height == 0) *grid = (character_grid_t){ 0 };
	else {
		grid->width = width;
		grid->height = height;
		grid->size = width * height;
		grid->chars = calloc_s(grid->size * sizeof(character_t));
	}
}
void character_grid_free(character_grid_t *grid) {
	assert_s(grid && "[character_grid_free] grid == NULL");
	free_s(grid->chars);
	*grid = (character_grid_t){ 0 };
}
character_t *character_grid_get(const character_grid_t *grid, size_t x, size_t y) {
	assert_s(grid && "[character_grid_get] grid == NULL");
	if (grid->chars && x < grid->width && y < grid->height)
		return grid->chars + x + y * grid->width;
	return NULL;
}

void character_grid_test_fill(character_grid_t *grid) {
	assert_s(grid && "[character_grid_test_fill] grid == NULL");
	if (grid->width == 0 || grid->height == 0 || grid->size == 0 || grid->chars == NULL)
		errout("cannot test fill empty grid");
	else
		for (size_t i = 0; i < grid->size; ++i)
			grid->chars[i] = (character_t){ .character = (uint8_t)i, .colour = (uint8_t)i  };
}
