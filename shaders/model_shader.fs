#version 330

in vec2 texture_coordinates;

out vec4 frag_colour;

uniform sampler2D texture_diffuse1;

void main() { frag_colour = texture(texture_diffuse1, texture_coordinates); }