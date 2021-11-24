#include "material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                   float shininess)
    : ambient_(ambient),
      diffuse_(diffuse),
      specular_(specular),
      shininess_(shininess) {}

void Material::Set(std::string name, Shader& shader) {
  shader.SetUniform3f(name + ".ambient", ambient_);
  shader.SetUniform3f(name + ".diffuse", diffuse_);
  shader.SetUniform3f(name + ".specular", specular_);
  shader.SetUniform1f(name + ".shininess", shininess_);
}
