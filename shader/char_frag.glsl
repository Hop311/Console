#version 330 core

in vec2 uv_frag;
flat in uint colour_frag;

out vec3 colour_out;

uniform usampler2D charsheet;
uniform vec3 colours[16];

void main(void) {
	float tex = texture(charsheet, uv_frag).r;
	colour_out = tex == 0.0f ? colours[colour_frag & 15U] : tex * colours[colour_frag >> 4U];
}
