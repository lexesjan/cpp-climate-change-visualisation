#version 460

in vec2 texture_coordinates;

out vec4 frag_colour;

struct Material {
  sampler2D texture_diffuse1;
  vec3 ambient;
  vec3 diffuse;
};

uniform Material material;

void main() {
  frag_colour = texture(material.texture_diffuse1, texture_coordinates) +
                vec4(material.diffuse, 1.0f);
}