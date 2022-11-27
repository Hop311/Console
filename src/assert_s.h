#pragma once

//#define NDEBUG

#include <assert.h>

#define assert_s assert

#define STATIC_ASSERT(expr, msg) \
	extern char STATIC_ASSERTION__##msg[1]; \
	extern char STATIC_ASSERTION__##msg[(expr)?1:2]
