#include "assimp_utils.h"

glm::mat4 assimp_utils::ConvertToMat4(aiMatrix4x4& matrix) {
  glm::mat4 output;

  output[0][0] = matrix.a1;
  output[1][0] = matrix.a2;
  output[2][0] = matrix.a3;
  output[3][0] = matrix.a4;
  output[0][1] = matrix.b1;
  output[1][1] = matrix.b2;
  output[2][1] = matrix.b3;
  output[3][1] = matrix.b4;
  output[0][2] = matrix.c1;
  output[1][2] = matrix.c2;
  output[2][2] = matrix.c3;
  output[3][2] = matrix.c4;
  output[0][3] = matrix.d1;
  output[1][3] = matrix.d2;
  output[2][3] = matrix.d3;
  output[3][3] = matrix.d4;

  return output;
}

glm::vec3 assimp_utils::ConvertToVec3(aiVector3D& position) {
  return glm::vec3(position.x, position.y, position.z);
}

glm::quat assimp_utils::ConvertToQuat(aiQuaternion& quaternion) {
  return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
}
