#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

flat in uint char_geom[];

out vec2 uv_frag;
flat out uint colour_frag;

uniform vec2 char_dims;

const float charsheet_dim = 1.0f / 16.0f;
const float edge_correction = charsheet_dim * charsheet_dim * charsheet_dim;

vec2 char_to_uv(uint c) {
	return vec2(float(c%16U), float(c/16U)) * charsheet_dim;
}

void main(void) {
	colour_frag = char_geom[0] >> 8U;

	vec2 uv_char = char_to_uv(char_geom[0] & 255u);

	gl_Position = gl_in[0].gl_Position;
	uv_frag = uv_char + edge_correction;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.y -= char_dims.y;
	uv_frag = uv_char + vec2(edge_correction, charsheet_dim - edge_correction);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.x += char_dims.x;
	uv_frag = uv_char + vec2(charsheet_dim - edge_correction, edge_correction);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.x += char_dims.x;
	gl_Position.y -= char_dims.y;
	uv_frag = uv_char + (charsheet_dim - edge_correction);
	EmitVertex();

	EndPrimitive();
}
