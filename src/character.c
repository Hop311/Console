#include "character.h"

#include "assert_s.h"
#include "memory_s.h"
#include "logging.h"

void character_grid_init(character_grid_t *grid, uvec2 dims) {
	assert_s(grid && "[character_grid_init] grid == NULL");
	character_grid_free(grid);
	const uint32_t size = dims.width * dims.height;
	if (size == 0) *grid = (character_grid_t){ 0 };
	else *grid = (character_grid_t){ dims, size, calloc_s(size * sizeof(character_t)) };
}
void character_grid_free(character_grid_t *grid) {
	assert_s(grid && "[character_grid_free] grid == NULL");
	free_s(grid->chars);
	*grid = (character_grid_t){ 0 };
}
character_t *character_grid_get(const character_grid_t *grid, uvec2 point) {
	assert_s(grid && "[character_grid_get] grid == NULL");
	if (grid->chars && point.x < grid->dims.width && point.y < grid->dims.height)
		return grid->chars + point.x + point.y * grid->dims.width;
	return NULL;
}
