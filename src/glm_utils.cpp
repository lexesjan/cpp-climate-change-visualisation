#include "glm_utils.h"
#include <glm/geometric.hpp>

glm::vec2 glm_utils::set_length(glm::vec2 input, float length) {
  glm::vec2 result = glm::normalize(input);

  return result * length;
}

glm::vec2 glm_utils::limit(glm::vec2& input, float limit) {
  if (glm::length(input) > limit) {
    glm::vec2 result = glm::normalize(input);
    result *= limit;

    return result;
  }

  return glm::vec2(input);
}
