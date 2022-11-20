#version 330 core

layout(location = 0) in uint char_in;

flat out uint char_geom;

uniform int width;
uniform vec2 char_dims, top_left;

void main(void) {
	char_geom = char_in;
	gl_Position = vec4(top_left + char_dims * vec2(gl_VertexID % width, -gl_VertexID / width), 0.0f, 1.0f);
}
