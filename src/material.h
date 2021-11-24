#ifndef CLIMATE_CHANGE_VISUALISATION_MATERIAL_H_
#define CLIMATE_CHANGE_VISUALISATION_MATERIAL_H_

#include <glm/ext/vector_float3.hpp>
#include <string>
#include "shader.h"

class Material {
 public:
  explicit Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                    float shininess = 1.0f);

  void Set(std::string name, Shader& shader);

 private:
  glm::vec3 ambient_;
  glm::vec3 diffuse_;
  glm::vec3 specular_;
  float shininess_;
};

#endif
