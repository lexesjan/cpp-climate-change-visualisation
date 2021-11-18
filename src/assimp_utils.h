#ifndef CLIMATE_CHANGE_VISUALISATION_ASSIMP_UTILS_H_
#define CLIMATE_CHANGE_VISUALISATION_ASSIMP_UTILS_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>

namespace assimp_utils {

glm::mat4 ConvertToMat4(aiMatrix4x4& matrix);

glm::vec3 ConvertToVec3(aiVector3D& position);

glm::quat ConvertToQuat(aiQuaternion& quaternion);

}  // namespace assimp_utils

#endif  // CLIMATE_CHANGE_VISUALISATION_ASSIMP_UTILITIES_H_
