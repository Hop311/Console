#include "memory_s.h"

#include "logging.h"
#include "assert_s.h"

#include <inttypes.h>
#include <stdlib.h>

static int64_t malloc_count = 0;
static int64_t realloc_count = 0;
static int64_t free_count = 0;

void *malloc_s(size_t size) {
	if (size == 0) {
		warnout("size 0 request");
		return 0;
	}
	void *ret = malloc(size);
	assert_s(ret && "[malloc_s] malloc failed");
	malloc_count++;
	return ret;
}
void *calloc_s(size_t size) {
	if (size == 0) {
		warnout("size 0 request");
		return 0;
	}
	void *ret = calloc(size, 1);
	assert_s(ret && "[calloc_s] calloc failed");
	malloc_count++;
	return ret;
}
void *realloc_s(void *ptr, size_t size) {
	if (size == 0) {
		free_s(ptr);
		warnout("size 0 request");
		return 0;
	}
	void *ret = realloc(ptr, size);
	assert_s(ret && "[realloc_s] realloc failed");
	if (ptr) realloc_count++;
	else {
		//dbgout("ptr == 0");
		malloc_count++;
	}
	return ret;
}
void free_s(void *ptr) {
	if (ptr) {
		free(ptr);
		free_count++;
	}
}

void check_memory_leaks(void) {
	dbgout("malloc_count = %"PRId64", realloc_count = %"PRId64", free_count = %"PRId64, malloc_count, realloc_count, free_count);
	if (malloc_count - free_count)
		errout("malloc_count - free_count = %"PRId64, malloc_count - free_count);
}
