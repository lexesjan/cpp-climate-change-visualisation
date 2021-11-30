#version 460

layout(location = 0) in vec3 vert_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() { gl_Position = proj * view * model * vec4(vert_position, 1.0f); }
