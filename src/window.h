#pragma once

typedef void (*TickFunction)();
typedef void (*RenderFunction)();

int window_init(int width, int height, const char *title);
void window_deinit(void);
void window_loop(TickFunction tick, RenderFunction render);
