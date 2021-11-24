#ifndef CLIMATE_CHANGE_VISUALISATION_DIRECTED_LIGHT_H_
#define CLIMATE_CHANGE_VISUALISATION_DIRECTED_LIGHT_H_

#include <glm/vec3.hpp>
#include "shader.h"

class DirectedLight {
 public:
  explicit DirectedLight(glm::vec3 colour, glm::vec3 direction);

  void Set(std::string name, Shader& shader);

 private:
  glm::vec3 colour_;
  glm::vec3 direction_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_DIRECTED_LIGHT_H_
