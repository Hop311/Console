#include "colour.h"

const colour_t COLOURS[COLOUR_COUNT] = {
	{{ 0.0f, 0.0f, 0.0f }},	// black
	{{ 0.0f, 0.0f, 0.5f }},	// blue
	{{ 0.0f, 0.5f, 0.0f }},	// green
	{{ 0.0f, 0.5f, 0.5f }},	// cyan
	{{ 0.5f, 0.0f, 0.0f }},	// red
	{{ 0.5f, 0.0f, 0.5f }},	// magenta
	{{ 0.5f, 0.5f, 0.0f }},	// brown
	{{ 0.75f,0.75f,0.75f }},// light grey
	{{ 0.5f, 0.5f, 0.5f }},	// dark grey
	{{ 0.0f, 0.0f, 1.0f }},	// light blue
	{{ 0.0f, 1.0f, 0.0f }},	// light green
	{{ 0.0f, 1.0f, 1.0f }},	// light cyan
	{{ 1.0f, 0.0f, 0.0f }},	// light red
	{{ 1.0f, 0.0f, 1.0f }},	// light magenta
	{{ 1.0f, 1.0f, 0.0f }},	// yellow
	{{ 1.0f, 1.0f, 1.0f }}	// white
};

uint8_t colour_pack(uint8_t foreground, uint8_t background) {
	return (foreground << 4) | (background & 15);
}
uint8_t colour_foreground(uint8_t colour) {
	return colour >> 4;
}
uint8_t colour_background(uint8_t colour) {
	return colour & 15;
}
uint8_t colour_invert(uint8_t colour) {
	return (colour >> 4) | (colour << 4);
}
