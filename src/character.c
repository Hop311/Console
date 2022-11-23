#include "character.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"

void character_grid_init(character_grid_t *grid, uint32_t width, uint32_t height) {
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
character_t *character_grid_get(const character_grid_t *grid, uint32_t x, uint32_t y) {
	assert_s(grid && "[character_grid_get] grid == NULL");
	if (grid->chars && x < grid->width && y < grid->height)
		return grid->chars + x + y * grid->width;
	return NULL;
}
