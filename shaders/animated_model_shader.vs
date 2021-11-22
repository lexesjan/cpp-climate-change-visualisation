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
uniform mat4 final_bone_transforms[MAX_BONES];

out vec2 texture_coordinates;

// --------------------------------------------------------------------------------
out vec3 light_intensity;

vec4 light_position =
    vec4(10.0, 10.0, 4.0, 1.0);  // Light position in world coords.
vec3 kd = vec3(0.0, 0.8, 0.0);   // green diffuse surface reflectance
vec3 ld = vec3(1.0, 1.0, 1.0);   // Light source intensity
// --------------------------------------------------------------------------------

void main() {
  texture_coordinates = tex_coords;

  mat4 bone_transform = final_bone_transforms[bone_ids[0]] * weights[0];
  bone_transform += final_bone_transforms[bone_ids[1]] * weights[1];
  bone_transform += final_bone_transforms[bone_ids[2]] * weights[2];
  bone_transform += final_bone_transforms[bone_ids[3]] * weights[3];

  vec4 pos_l = bone_transform * vec4(vertex_position, 1.0f);

  gl_Position = proj * view * model * pos_l;

  // --------------------------------------------------------------------------------
  mat4 mv_mat = view * model;
  mat3 normal_mat = mat3(mv_mat);
  // Convert normal and position to eye coords
  // Normal in view space
  vec3 tnorm = normalize(normal_mat * vertex_normals);
  // Position in view space
  vec4 eye_coords = mv_mat * vec4(vertex_position, 1.0);
  // normalised vector towards the light source
  vec3 s = normalize(vec3(light_position - eye_coords));

  // The diffuse shading equation, dot product gives us the cosine of angle
  // between the vectors
  light_intensity = ld * kd * max(dot(s, tnorm), 0.0);
  // --------------------------------------------------------------------------------
}
