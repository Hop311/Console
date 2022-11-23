#pragma once

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHT_GREY		7
#define DARK_GREY		8
#define LIGHT_BLUE		9
#define LIGHT_GREEN		10
#define LIGHT_CYAN		11
#define LIGHT_RED		12
#define LIGHT_MAGENTA	13
#define YELLOW			14
#define WHITE			15

#define PACK_COLOURS(foreground, background) ((foreground & 15) << 4) | (background & 15)

extern const float COLOURS[16][3];
