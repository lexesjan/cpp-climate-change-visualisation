#version 330

in vec2 texture_coordinates;
in vec3 vertex_position_in_world_space;
in vec3 normal;

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
uniform vec3 view_position;

void main() {
  vec3 object_colour =
      vec3(texture(material.texture_diffuse1, texture_coordinates));

  // Ambient.
  float ambient_strength = 0.1f;
  vec3 ambient = ambient_strength * light.colour;

  // Diffuse.
  vec3 normalised_normal = normalize(normal);
  vec3 light_direction =
      normalize(light.position - vertex_position_in_world_space);
  float diff = max(dot(normalised_normal, light_direction), 0.0f);
  vec3 diffuse = diff * light.colour;

  // Specular.
  float specular_strength = 0.5f;
  vec3 view_direction =
      normalize(view_position - vertex_position_in_world_space);
  vec3 reflection_direction = reflect(-light_direction, normalised_normal);
  float spec = pow(max(dot(view_direction, reflection_direction), 0.0f),
                   material.shininess);
  vec3 specular = specular_strength * spec * light.colour;

  frag_colour = vec4((ambient + diffuse + specular) * object_colour, 1.0f);
}