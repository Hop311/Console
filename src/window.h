#pragma once

typedef void (*TickFunction)(void);
typedef void (*RenderFunction)(void);

int window_init(int width, int height, const char *title);
void window_deinit(void);
void window_loop(TickFunction tick, RenderFunction render);
