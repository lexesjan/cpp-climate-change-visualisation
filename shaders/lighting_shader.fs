#version 330

in vec2 texture_coordinates;
in vec3 vertex_position_in_world_space;
in vec3 normals;

out vec4 frag_colour;

struct Material {
  sampler2D texture_diffuse1;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 colour;
};

uniform Material material;
uniform Light light;

void main() {
  vec3 object_colour =
      vec3(texture(material.texture_diffuse1, texture_coordinates));

  // Ambient.
  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light.colour;

  // Diffuse.
  vec3 normalised_normals = normalize(normals);
  vec3 light_direction =
      normalize(light.position - vertex_position_in_world_space);
  float diff = max(dot(normalised_normals, light_direction), 0.0);
  vec3 diffuse = diff * light.colour;

  frag_colour = vec4((ambient + diffuse) * object_colour, 1.0f);
}