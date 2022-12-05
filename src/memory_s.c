#include "memory_s.h"

#include "logging.h"
#include "assert_s.h"
#include "maths.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdarg.h>

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

void *buf__grow(const void *buf, size_t new_len, size_t elem_size) {
	assert_s(buf_cap(buf) <= (SIZE_MAX - 1) / 2 && "[buf__grow] capacity will overflow");
	size_t new_cap = MAX(2 * buf_cap(buf), MAX(new_len, 16));
	assert_s(new_len <= new_cap && "[buf__grow] capacity will be less than length");
	assert_s(new_cap <= (SIZE_MAX - offsetof(buffer_t, buf)) / elem_size && "[buf__grow] capacity will overflow");
	size_t new_size = offsetof(buffer_t, buf) + new_cap * elem_size;
	buffer_t *new_buf;
	if (buf) new_buf = realloc_s(buf__hdr(buf), new_size);
	else {
		new_buf = malloc_s(new_size);
		new_buf->len = 0;
	}
	new_buf->cap = new_cap;
	return new_buf->buf;
}
char *buf__printf(char *buf, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t cap = buf_cap(buf) - buf_len(buf);
	size_t n = 1 + vsnprintf(buf_end(buf), cap, fmt, args);
	va_end(args);
	if (n > cap) {
		buf_fit(buf, n + buf_len(buf));
		va_start(args, fmt);
		cap = buf_cap(buf) - buf_len(buf);
		n = 1 + vsnprintf(buf_end(buf), cap, fmt, args);
		assert_s(n <= cap);
		va_end(args);
	}
	buf__hdr(buf)->len += n - 1;
	return buf;
}
