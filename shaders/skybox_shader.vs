#version 460

layout(location = 0) in vec3 vert_position;

out vec3 texture_coordinates;

uniform mat4 proj;
uniform mat4 view;

void main() {
  texture_coordinates = vert_position;
  gl_Position = proj * view * vec4(vert_position, 1.0);
}