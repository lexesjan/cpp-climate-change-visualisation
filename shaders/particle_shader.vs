#version 460

layout(location = 0) in vec3 vert_position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

int main() { gl_Position = prod * view * model * vec4(vert_position, 1.0f); }
