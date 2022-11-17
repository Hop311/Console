#pragma once

#include <stddef.h>

void *malloc_s(size_t size);
void *calloc_s(size_t size);
void *realloc_s(void *ptr, size_t size);
void free_s(void *ptr);

void check_memory_leaks(void);
