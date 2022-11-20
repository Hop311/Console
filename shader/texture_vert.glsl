#version 330 core

layout(location = 0) in vec2 pos_in;
layout(location = 1) in vec2 uv_in;

out vec2 uv_frag;

void main(void) {
	gl_Position = vec4(pos_in, 0.0f, 1.0f);
	uv_frag = uv_in;
}
