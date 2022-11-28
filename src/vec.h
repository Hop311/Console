#pragma once

#include "assert_s.h"

#include <stdint.h>

#define VEC2(type, name) typedef union { struct { type x, y; }; struct { type width, height; }; type v[2]; } name; \
    STATIC_ASSERT(sizeof(name) == sizeof(type[2]), sizeof_##name);
VEC2(int32_t, ivec2)
VEC2(uint32_t, uvec2)
VEC2(float, fvec2)
#undef VEC2

#define VEC3(type, name) typedef union { struct { type x, y, z; }; struct { type width, height, depth; }; struct { type r, g, b; }; type v[3]; } name; \
    STATIC_ASSERT(sizeof(name) == sizeof(type[3]), sizeof_##name);
VEC3(float, colour_t)
#undef VEC3
