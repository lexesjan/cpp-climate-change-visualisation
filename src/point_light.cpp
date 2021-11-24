#include "point_light.h"

PointLight::PointLight(

    glm::vec3 colour, glm::vec3 position, float constant, float linear,
    float quadratic)
    : colour_(colour),
      position_(position),
      constant_(constant),
      linear_(linear),
      quadratic_(quadratic) {}

void PointLight::Set(std::string name, Shader& shader, unsigned int index) {
  shader.SetUniform3f(name + "[" + std::to_string(index) + "].position",
                      position_);
  shader.SetUniform3f(name + "[" + std::to_string(index) + "].ambient",
                      colour_ * 0.1f);
  shader.SetUniform3f(name + "[" + std::to_string(index) + "].specular",
                      colour_);
  shader.SetUniform3f(name + "[" + std::to_string(index) + "].diffuse",
                      colour_);
  shader.SetUniform1f(name + "[" + std::to_string(index) + "].constant",
                      constant_);
  shader.SetUniform1f(name + "[" + std::to_string(index) + "].linear", linear_);
  shader.SetUniform1f(name + "[" + std::to_string(index) + "].quadratic",
                      quadratic_);
}

glm::vec3& PointLight::GetPosition() { return position_; }
