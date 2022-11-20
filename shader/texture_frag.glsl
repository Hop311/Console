#version 330 core

in vec2 uv_frag;

out vec4 colour_out;

uniform sampler2D image;

void main(void) {
	colour_out = texture(image, uv_frag);
}
