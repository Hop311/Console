#pragma once

#include <stdio.h>

const char *get_filename(const char *path);

#define print_log(str,...) printf("[%s|%s|%d]"str"\n\n",__func__,get_filename(__FILE__),__LINE__,##__VA_ARGS__)

#define dbgout(str,...) print_log("[DEBUG] "str,##__VA_ARGS__)
#define warnout(str,...) print_log("[WARNING] "str,##__VA_ARGS__)
#define errout(str,...) print_log("[ERROR] "str,##__VA_ARGS__)
