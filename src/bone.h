#ifndef CLIMATE_CHANGE_VISUALISATION_BONE_H_
#define CLIMATE_CHANGE_VISUALISATION_BONE_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <assimp/anim.h>
#include <vector>
#include <string>

struct KeyPosition {
  glm::vec3 position;
  float time_stamp;
};

struct KeyRotation {
  glm::quat orientation;
  float time_stamp;
};

struct KeyScale {
  glm::vec3 scale;
  float time_stamp;
};

class Bone {
 public:
  explicit Bone(const std::string& name, int id, const aiNodeAnim* channel);

  void Update(float animation_time);

  int GetPositionIndex(float animation_time);

  int GetRotationIndex(float animation_time);

  int GetScaleIndex(float animation_time);

 private:
  std::vector<KeyPosition> positions_;
  std::vector<KeyRotation> rotations_;
  std::vector<KeyScale> scales_;
  int num_positions_;
  int num_rotations_;
  int num_scalings_;
  glm::mat4 local_transform_;
  std::string name_;
  int id_;

  float GetScaleFactor(float last_time_stamp, float next_time_stamp,
                       float animation_time) const;

  glm::mat4 InterpolatePosition(float animation_time);

  glm::mat4 InterpolateRotation(float animation_time);

  glm::mat4 InterpolateScaling(float animation_time);
};

#endif  // CLIMATE_CHANGE_VISUALISATION_BONE_H_
