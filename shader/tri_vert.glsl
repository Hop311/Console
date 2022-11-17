#version 330 core

layout(location = 0) in vec2 pos_in;
layout(location = 1) in vec3 colour_in;

out vec3 colour_frag;

void main() {
	gl_Position = vec4(pos_in, 0.0f, 1.0f);
	colour_frag = colour_in;
}
