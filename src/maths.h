#pragma once

#include <string.h>

#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define CLAMP(x, min, max) ((x) <= (min) ? (min) : (x) >= (max) ? (max) : (x))
#define SWAP(x, y) do { \
	unsigned char tmp[sizeof(x) == sizeof(y) ? (int)sizeof(x) : -1]; \
	memcpy(tmp, &(x), sizeof(x)); \
	memcpy(&(x), &(y), sizeof(x)); \
	memcpy(&(y), tmp, sizeof(x)); } while (0)
