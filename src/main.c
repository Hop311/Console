﻿#include <stdio.h>

#include "window.h"
#include "logging.h"
#include "memory_s.h"

#include "character.h"

void tick() {

}

void render() {

}

int main() {
	int ret = window_init(800, 600, "Console");
	if (ret) return ret;
	window_loop(tick, render);
	window_deinit();
	check_memory_leaks();
	return 0;
}
