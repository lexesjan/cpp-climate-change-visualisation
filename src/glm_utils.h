#ifndef CLIMATE_CHANGE_VISUALISATION_GLM_UTILS_H_
#define CLIMATE_CHANGE_VISUALISATION_GLM_UTILS_H_

#include <glm/vec2.hpp>

namespace glm_utils {

glm::vec2 set_length(glm::vec2 input, float length);

glm::vec2 limit(glm::vec2& input, float limit);

}  // namespace glm_utils

#endif  //  CLIMATE_CHANGE_VISUALISATION_GLM_UTILS_H_