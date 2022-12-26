#pragma once

#include "vec.h"

#define COLOUR_COUNT	16

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

extern const colour_t COLOURS[COLOUR_COUNT];

uint8_t colour_pack(uint8_t foreground, uint8_t background);
uint8_t colour_foreground(uint8_t colour);
uint8_t colour_background(uint8_t colour);
uint8_t colour_invert(uint8_t colour);
