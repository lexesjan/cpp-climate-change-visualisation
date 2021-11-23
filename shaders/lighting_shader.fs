#version 330

in vec2 texture_coordinates;
in vec2 vertex_normals;

out vec4 frag_colour;

struct AmbientLight {
  vec3 colour;
  float intensity;
};

struct Material {
  sampler2D texture_diffuse1;
};

uniform Material material;
uniform AmbientLight light;

void main() {
  vec4 ambient = vec4(light.colour * light.intensity, 1.0f);

  frag_colour =
      texture(material.texture_diffuse1, texture_coordinates) * ambient;
}