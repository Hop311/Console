#pragma once

#include <stdio.h>

const char *get_filename(const char *path);

#define print(str,...) printf("[%s|%s|%d] "str,__func__,get_filename(__FILE__),__LINE__,##__VA_ARGS__)

#define dbgout(str,...) print("[DEBUG] "str,##__VA_ARGS__)
#define warnout(str,...) print("[WARNING] "str,##__VA_ARGS__)
#define errout(str,...) print("[ERROR] "str,##__VA_ARGS__)
