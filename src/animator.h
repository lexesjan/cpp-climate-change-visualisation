#ifndef CLIMATE_CHANGE_VISUALISATION_ANIMATOR_H_
#define CLIMATE_CHANGE_VISUALISATION_ANIMATOR_H_

#include "animation.h"

class Animator {
 public:
  explicit Animator(Animation* animation);

  void UpdateAnimation(float delta);

  void PlayAnimation(Animation* animation);

  void CalculateBoneTransform(const AssimpNodeData* node,
                              glm::mat4 parent_transform);

  const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

 private:
  std::vector<glm::mat4> final_bone_matrices_;
  Animation* current_animation_;
  float current_time_;
  float delta_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_ANIMATOR_H_
