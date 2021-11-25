#ifndef CLIMATE_CHANGE_VISUALISATION_LIGHT_SOURCE_H_
#define CLIMATE_CHANGE_VISUALISATION_LIGHT_SOURCE_H_

#include "point_light.h"
#include "model.h"

class LightSource : public Model, public PointLight {
 public:
  explicit LightSource(std::string path, glm::vec3 colour, glm::vec3 position,
                       Shader& shader, Renderer& renderer,
                       float constant = 1.0f, float linear = 0.5f,
                       float quadratic = 0.05f,
                       Material material = Material(glm::vec3(0.0f),
                                                    glm::vec3(0.0f),
                                                    glm::vec3(0.0f)));

  void Draw() override;

 private:
  glm::mat4 model_matrix_;
};

#endif