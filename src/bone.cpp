#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include "bone.h"
#include "assimp_utils.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    : name_(name), id_(ID), local_transform_(1.0f) {
  num_positions_ = channel->mNumPositionKeys;

  for (unsigned int i = 0; i < num_positions_; i++) {
    aiVector3D position = channel->mPositionKeys[i].mValue;

    float time_stamp = channel->mPositionKeys[i].mTime;

    KeyPosition data;
    data.position = assimp_utils::ConvertToVec3(position);
    data.time_stamp = time_stamp;

    positions_.push_back(data);
  }

  num_rotations_ = channel->mNumRotationKeys;

  for (int i = 0; i < num_rotations_; i++) {
    aiQuaternion orientation = channel->mRotationKeys[i].mValue;

    float time_stamp = channel->mRotationKeys[i].mTime;

    KeyRotation data;
    data.orientation = assimp_utils::ConvertToQuat(orientation);
    data.time_stamp = time_stamp;

    rotations_.push_back(data);
  }

  num_scalings_ = channel->mNumScalingKeys;

  for (int i = 0; i < num_scalings_; i++) {
    aiVector3D scale = channel->mScalingKeys[i].mValue;
    float time_stamp = channel->mScalingKeys[i].mTime;

    KeyScale data;
    data.scale = assimp_utils::ConvertToVec3(scale);
    data.time_stamp = time_stamp;

    scales_.push_back(data);
  }
}

void Bone::Update(float animationTime) {
  glm::mat4 translation = InterpolatePosition(animationTime);
  glm::mat4 rotation = InterpolateRotation(animationTime);
  glm::mat4 scale = InterpolateScaling(animationTime);

  local_transform_ = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animation_time) {
  for (int index = 0; index < num_positions_ - 1; ++index) {
    if (animation_time < positions_[index + 1].time_stamp) {
      return index;
    }
  }

  return -1;
}

int Bone::GetRotationIndex(float animation_time) {
  for (int index = 0; index < num_rotations_ - 1; ++index) {
    if (animation_time < rotations_[index + 1].time_stamp) {
      return index;
    }
  }

  return -1;
}

int Bone::GetScaleIndex(float animation_time) {
  for (int index = 0; index < num_scalings_ - 1; ++index) {
    if (animation_time < scales_[index + 1].time_stamp) {
      return index;
    }
  }

  return -1;
}

const std::string& Bone::GetBoneName() const { return name_; }

const glm::mat4& Bone::GetLocalTransform() const { return local_transform_; }

float Bone::GetScaleFactor(float last_time_stamp, float next_time_stamp,
                           float animation_time) const {
  float mid_way_length = animation_time - last_time_stamp;
  float frames_difference = next_time_stamp - last_time_stamp;

  return mid_way_length / frames_difference;
}

glm::mat4 Bone::InterpolatePosition(float animation_time) {
  if (1 == num_positions_) {
    return glm::translate(glm::mat4(1.0f), positions_[0].position);
  }

  int p0_index = GetPositionIndex(animation_time);
  int p1_index = p0_index + 1;
  float scale_factor =
      GetScaleFactor(positions_[p0_index].time_stamp,
                     positions_[p1_index].time_stamp, animation_time);
  glm::vec3 final_position =
      glm::mix(positions_[p0_index].position, positions_[p1_index].position,
               scale_factor);
  return glm::translate(glm::mat4(1.0f), final_position);
}

glm::mat4 Bone::InterpolateRotation(float animation_time) {
  if (1 == num_rotations_) {
    glm::quat rotation = glm::normalize(rotations_[0].orientation);

    return glm::toMat4(rotation);
  }

  int p0_index = GetRotationIndex(animation_time);
  int p1_index = p0_index + 1;

  float scale_factor =
      GetScaleFactor(rotations_[p0_index].time_stamp,
                     rotations_[p1_index].time_stamp, animation_time);

  glm::quat final_rotation =
      glm::slerp(rotations_[p0_index].orientation,
                 rotations_[p1_index].orientation, scale_factor);
  final_rotation = glm::normalize(final_rotation);

  return glm::toMat4(final_rotation);
}

glm::mat4 Bone::InterpolateScaling(float animation_time) {
  if (1 == num_scalings_) {
    return glm::scale(glm::mat4(1.0f), scales_[0].scale);
  }

  int p0_index = GetScaleIndex(animation_time);
  int p1_index = p0_index + 1;

  float scale_factor =
      GetScaleFactor(scales_[p0_index].time_stamp, scales_[p1_index].time_stamp,
                     animation_time);

  glm::vec3 final_scale =
      glm::mix(scales_[p0_index].scale, scales_[p1_index].scale, scale_factor);

  return glm::scale(glm::mat4(1.0f), final_scale);
}
