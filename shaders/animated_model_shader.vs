#version 460

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 tex_coords;
layout(location = 3) in ivec4 bone_ids;
layout(location = 4) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final_bone_transforms[MAX_BONES];

out vec2 texture_coordinates;
out vec3 translated_vertex_position;
out vec3 translated_normal;

void main() {
  texture_coordinates = tex_coords;

  mat4 bone_transform = final_bone_transforms[bone_ids[0]] * weights[0];
  bone_transform += final_bone_transforms[bone_ids[1]] * weights[1];
  bone_transform += final_bone_transforms[bone_ids[2]] * weights[2];
  bone_transform += final_bone_transforms[bone_ids[3]] * weights[3];

  vec4 pos_l = bone_transform * vec4(vert_position, 1.0f);

  gl_Position = proj * view * model * pos_l;

  translated_vertex_position = vec3(model * vec4(vert_position, 1.0f));
  translated_normal = mat3(transpose(inverse(bone_transform * model))) * vert_normal;
  texture_coordinates = tex_coords;
}
