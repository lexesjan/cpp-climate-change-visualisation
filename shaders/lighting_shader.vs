#version 330

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normals;
layout(location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texture_coordinates;
out vec3 vertex_position_in_world_space;
out vec3 normals;

void main() {
  vertex_position_in_world_space = vec3(model * vec4(vert_position, 1.0f));
  normals = mat3(transpose(inverse(model))) * vert_normals;
  texture_coordinates = tex_coords;

  gl_Position = proj * view * model * vec4(vert_position, 1.0f);
}