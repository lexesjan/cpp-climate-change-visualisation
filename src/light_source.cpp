#include <glm/gtc/type_ptr.hpp>
#include "light_source.h"

LightSource::LightSource(std::string path, glm::vec3 colour, glm::vec3 position,
                         Shader& shader, Renderer& renderer, float constant,
                         float linear, float quadratic, Material material)
    : PointLight(colour, position, constant, linear, quadratic),
      Model(path, shader, renderer, material),
      model_matrix_(glm::translate(glm::mat4(1.0f), glm::vec3(position))) {}

void LightSource::Draw() {
  shader_.SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model_matrix_));

  Model::Draw();
}
