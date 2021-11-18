#include "animator.h"

Animator::Animator(Animation* animation) : delta_(0.0f) {
  current_time_ = 0.0;
  current_animation_ = animation;

  final_bone_matrices_.reserve(100);

  for (int i = 0; i < 100; i++) {
    final_bone_matrices_.push_back(glm::mat4(1.0f));
  }
}

void Animator::UpdateAnimation(float delta) {
  delta_ = delta;

  if (!current_animation_) {
    return;
  }

  current_time_ += current_animation_->GetTicksPerSecond() * delta;
  current_time_ = fmod(current_time_, current_animation_->GetDuration());
  CalculateBoneTransform(&current_animation_->GetRootNode(), glm::mat4(1.0f));
}

void Animator::PlayAnimation(Animation* animation) {
  current_animation_ = animation;
  current_time_ = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node,
                                      glm::mat4 parent_transform) {
  std::string node_name = node->name;
  glm::mat4 node_transformation = node->transformation;

  Bone* bone = current_animation_->FindBone(node_name);

  if (bone) {
    bone->Update(current_time_);
    node_transformation = bone->GetLocalTransform();
  }

  glm::mat4 global_transformation_ = parent_transform * node_transformation;

  auto bone_info_map = current_animation_->GetBoneInfoMap();

  if (bone_info_map.find(node_name) != bone_info_map.end()) {
    glm::mat4 offset = bone_info_map[node_name].offset;

    int index = bone_info_map[node_name].id;
    final_bone_matrices_[index] = global_transformation_ * offset;
  }

  for (int i = 0; i < node->children_count; i++) {
    CalculateBoneTransform(&node->children[i], global_transformation_);
  }
}

const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices() const {
  return final_bone_matrices_;
}
