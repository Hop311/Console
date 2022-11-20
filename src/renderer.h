#pragma once

int renderer_init(void);
void renderer_deinit(void);
void renderer_resize(int width, int height, float scale);
void renderer_render(void);
