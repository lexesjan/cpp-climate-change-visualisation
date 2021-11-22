#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 tex_coords;

out vec2 texture_coordinates;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main() {
  texture_coordinates = tex_coords;
  gl_Position = proj * view * model * vec4(vertex_position, 1.0);
}