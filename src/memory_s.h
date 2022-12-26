#pragma once

#include <stddef.h>
#include <stdint.h>

void *malloc_s(size_t size);
void *calloc_s(size_t size);
void *realloc_s(void *ptr, size_t size);
void free_s(void *ptr);

void check_memory_leaks(void);

typedef struct {
	size_t len, cap;
	uint8_t buf[1];
} buffer_t;

#define buf__hdr(b) ((buffer_t *)((uint8_t *)(b) - offsetof(buffer_t, buf)))

#define buf_len(b) ((b) ? buf__hdr(b)->len : (size_t)0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : (size_t)0)
#define buf_end(b) ((b) + buf_len(b))
#define buf_back(b) (buf_end(b) - 1)
#define buf_sizeof(b) ((b) ? buf_len(b)*sizeof(*(b)) : (size_t)0)
#define buf_empty(b) (buf_len(b) == 0)

#define buf_free(b) ((b) ? (free_s(buf__hdr(b)), (b) = NULL) : 0)
#define buf_fit(b, n) ((n) <= buf_cap(b) ? 0 : ((b) = buf__grow((b), (n), sizeof(*(b)))))
#define buf_push(b, ...) (buf_fit((b), 1 + buf_len(b)), (b)[buf__hdr(b)->len++] = (__VA_ARGS__))
#define buf_printf(b, ...) ((b) = buf__printf((b), __VA_ARGS__))
#define buf_clear(b) ((b) ? buf__hdr(b)->len = 0 : 0)

void *buf__grow(const void *buf, size_t new_len, size_t elem_size);
char *buf__printf(char *buf, const char *fmt, ...);

#define for_buf(var, b) for (size_t var = 0; var < buf_len(b); ++var)
