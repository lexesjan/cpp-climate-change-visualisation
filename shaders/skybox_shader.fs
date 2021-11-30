#version 460

out vec4 frag_colour;

in vec3 texture_coordinates;

uniform samplerCube skybox;

void main() { frag_colour = texture(skybox, texture_coordinates); }