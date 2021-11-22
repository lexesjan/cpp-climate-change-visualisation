#ifndef CLIMATE_CHANGE_VISUALISATION_ANIMATED_MODEL_H_
#define CLIMATE_CHANGE_VISUALISATION_ANIMATED_MODEL_H_

#include "model.h"

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

struct AssimpNode {
  glm::mat4 transformation;
  std::string name;
  unsigned int children_count;
  std::vector<AssimpNode> children;
};

struct AssimpNodeAnimation {
  std::vector<KeyPosition> key_positions;
  std::vector<KeyRotation> key_rotations;
  std::vector<KeyScale> key_scales;
};

class AnimatedModel : public Model {
 public:
  explicit AnimatedModel(std::string path, Shader shader, Renderer renderer);
  explicit AnimatedModel(std::string model_path, std::string animation_path,
                         Shader shader, Renderer renderer);

  void UpdateBoneTransformations(bool backwards = false);

  const std::vector<glm::mat4> &GetFinalTransforms() const;

  void SetDelta(float delta);

 protected:
  float delta_;
  float current_time_;

 private:
  float ticks_per_second_;
  float animation_duration_;
  AssimpNode root_node_;
  std::vector<glm::mat4> final_transforms_;
  std::unordered_map<std::string, AssimpNodeAnimation> node_animations_map_;

  void LoadAnimations(std::string path);

  void ReadHeirarchyData(AssimpNode &destination, aiNode *source);

  void ProcessNodeHeirarchy(float animation_time, AssimpNode &node,
                            glm::mat4 parent_transform);

  void InitialiseNodeAnimationsMap(aiAnimation *animation);

  float GetScalingFactor(float last_time_stamp, float next_time_stamp,
                         float animation_time) const;

  const glm::mat4 InterpolateScaling(float animation_time,
                                     AssimpNodeAnimation &node_animation) const;

  const glm::mat4 InterpolateRotation(
      float animation_time, AssimpNodeAnimation &node_animation) const;

  const glm::mat4 InterpolateTranslation(
      float animation_time, AssimpNodeAnimation &node_animation) const;
};

#endif
