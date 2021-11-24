#include "directed_light.h"

DirectedLight::DirectedLight(glm::vec3 colour, glm::vec3 direction)
    : colour_(colour), direction_(direction) {}

void DirectedLight::Set(std::string name, Shader& shader) {
  shader.SetUniform3f(name + ".direction", direction_);
  shader.SetUniform3f(name + ".ambient", colour_ * 0.1f);
  shader.SetUniform3f(name + ".specular", colour_);
  shader.SetUniform3f(name + ".diffuse", colour_);
}
