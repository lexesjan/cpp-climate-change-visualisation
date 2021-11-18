#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 tex_coords;
layout(location = 3) in ivec4 bone_ids;
layout(location = 4) in vec4 weights;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final_bones_matrices[MAX_BONES];

out vec2 texture_coordinates;

void main() {
  vec4 total_position = vec4(0.0f);

  for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
    if (bone_ids[i] == -1) {
      continue;
    }

    if (bone_ids[i] >= MAX_BONES) {
      total_position = vec4(vertex_position, 1.0f);
      break;
    }

    vec4 local_position = final_bones_matrices[bone_ids[i]] * vec4(vertex_position, 1.0f);
    total_position += local_position * weights[i];

    // TODO: Use normals for lighting
    vec3 local_normal =
        mat3(final_bones_matrices[bone_ids[i]]) * vertex_normals;
  }

  texture_coordinates = tex_coords;
  gl_Position = proj * view * model * total_position;
}
