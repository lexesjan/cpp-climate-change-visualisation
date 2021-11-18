#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include "bone.h"
#include "assimp_utils.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    : name_(name), id_(ID), local_transform_(1.0f) {
  num_positions_ = channel->mNumPositionKeys;

  for (int positionIndex = 0; positionIndex < num_positions_; ++positionIndex) {
    aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;

    float time_stamp = channel->mPositionKeys[positionIndex].mTime;

    KeyPosition data;
    data.position = assimp_utils::ConvertToVec3(aiPosition);
    data.time_stamp = time_stamp;

    positions_.push_back(data);
  }

  num_rotations_ = channel->mNumRotationKeys;
  for (int rotationIndex = 0; rotationIndex < num_rotations_; ++rotationIndex) {
    aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;

    float time_stamp = channel->mRotationKeys[rotationIndex].mTime;

    KeyRotation data;
    data.orientation = assimp_utils::ConvertToQuat(aiOrientation);
    data.time_stamp = time_stamp;

    rotations_.push_back(data);
  }

  num_scalings_ = channel->mNumScalingKeys;

  for (int keyIndex = 0; keyIndex < num_scalings_; ++keyIndex) {
    aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
    float time_stamp = channel->mScalingKeys[keyIndex].mTime;

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

int Bone::GetPositionIndex(float animationTime) {
  for (int index = 0; index < num_positions_ - 1; ++index) {
    if (animationTime < positions_[index + 1].time_stamp) {
      return index;
    }
  }

  return -1;
}

int Bone::GetRotationIndex(float animationTime) {
  for (int index = 0; index < num_rotations_ - 1; ++index) {
    if (animationTime < rotations_[index + 1].time_stamp) {
      return index;
    }
  }

  return -1;
}

int Bone::GetScaleIndex(float animationTime) {
  for (int index = 0; index < num_scalings_ - 1; ++index) {
    if (animationTime < scales_[index + 1].time_stamp) {
      return index;
    }
  }

  return -1;
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp,
                           float animationTime) const {
  float midWayLength = animationTime - lastTimeStamp;
  float framesDiff = nextTimeStamp - lastTimeStamp;

  return midWayLength / framesDiff;
}

glm::mat4 Bone::InterpolatePosition(float animationTime) {
  if (1 == num_positions_) {
    return glm::translate(glm::mat4(1.0f), positions_[0].position);
  }

  int p0Index = GetPositionIndex(animationTime);
  int p1Index = p0Index + 1;
  float scaleFactor =
      GetScaleFactor(positions_[p0Index].time_stamp,
                     positions_[p1Index].time_stamp, animationTime);
  glm::vec3 finalPosition = glm::mix(positions_[p0Index].position,
                                     positions_[p1Index].position, scaleFactor);
  return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime) {
  if (1 == num_rotations_) {
    auto rotation = glm::normalize(rotations_[0].orientation);
    return glm::toMat4(rotation);
  }

  int p0Index = GetRotationIndex(animationTime);
  int p1Index = p0Index + 1;

  float scaleFactor =
      GetScaleFactor(rotations_[p0Index].time_stamp,
                     rotations_[p1Index].time_stamp, animationTime);

  glm::quat finalRotation =
      glm::slerp(rotations_[p0Index].orientation,
                 rotations_[p1Index].orientation, scaleFactor);
  finalRotation = glm::normalize(finalRotation);

  return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float animationTime) {
  if (1 == num_scalings_) {
    return glm::scale(glm::mat4(1.0f), scales_[0].scale);
  }

  int p0Index = GetScaleIndex(animationTime);
  int p1Index = p0Index + 1;

  float scaleFactor = GetScaleFactor(
      scales_[p0Index].time_stamp, scales_[p1Index].time_stamp, animationTime);

  glm::vec3 finalScale =
      glm::mix(scales_[p0Index].scale, scales_[p1Index].scale, scaleFactor);

  return glm::scale(glm::mat4(1.0f), finalScale);
}
