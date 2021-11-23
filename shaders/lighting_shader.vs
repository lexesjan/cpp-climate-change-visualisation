#version 330

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texture_coordinates;
out vec3 vertex_position_in_world_space;
out vec3 normal;

void main() {
  vertex_position_in_world_space = vec3(model * vec4(vert_position, 1.0f));
  normal = mat3(transpose(inverse(model))) * vert_normal;
  texture_coordinates = tex_coords;

  gl_Position = proj * view * vec4(vertex_position_in_world_space, 1.0f);
}