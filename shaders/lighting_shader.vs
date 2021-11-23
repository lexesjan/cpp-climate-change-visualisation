#version 330

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texture_coordinates;
out vec3 translated_vertex_position;
out vec3 translated_normal;

void main() {
  translated_vertex_position = vec3(model * vec4(vert_position, 1.0f));
  translated_normal = mat3(transpose(inverse(model))) * vert_normal;
  texture_coordinates = tex_coords;

  gl_Position = proj * view * vec4(translated_vertex_position, 1.0f);
}