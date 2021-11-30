#version 460

out vec4 frag_colour;

uniform vec4 colour;

int main() { frag_colour = colour; }