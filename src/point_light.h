#ifndef CLIMATE_CHANGE_VISUALISATION_POINT_LIGHT_H_
#define CLIMATE_CHANGE_VISUALISATION_POINT_LIGHT_H_

#include <glm/vec3.hpp>
#include "shader.h"

class PointLight {
 public:
  explicit PointLight(glm::vec3 colour, glm::vec3 position,
                      float constant = 1.0f, float linear = 0.1f,
                      float quadratic = 0.03f);

  void Set(std::string name, Shader& shader, unsigned int index = 0.0f);

  glm::vec3& GetPosition();

 private:
  float constant_;
  float linear_;
  float quadratic_;
  glm::vec3 colour_;
  glm::vec3 position_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_POINT_LIGHT_H_
