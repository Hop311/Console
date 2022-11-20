#version 330 core

in vec2 uv_frag;
flat in uint colour_frag;

out vec3 colour_out;

uniform usampler2D charsheet;
uniform vec3 colours[16];

void main(void) {
	uint tex = texture(charsheet, uv_frag).r;
	colour_out = bool(tex) ? colours[colour_frag >> 4U] : colours[colour_frag & 15U];
}
