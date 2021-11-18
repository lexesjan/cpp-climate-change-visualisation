#ifndef CLIMATE_CHANGE_VISUALISATION_ANIMATION_H_
#define CLIMATE_CHANGE_VISUALISATION_ANIMATION_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>
#include "model.h"
#include "bone.h"

struct AssimpNodeData {
  glm::mat4 transformation;
  std::string name;
  int children_count;
  std::vector<AssimpNodeData> children;
};

class Animation {
 public:
  explicit Animation(const std::string& animation_path, Model& model);

  Bone* FindBone(const std::string& name);

 private:
  float duration_;
  int ticks_per_second_;
  std::vector<Bone> bones_;
  AssimpNodeData root_node_;
  std::unordered_map<std::string, BoneInfo> bone_info_map_;

  void ReadMissingBones(const aiAnimation* animation, Model& model);

  void ReadHeirarchyData(AssimpNodeData& dest, aiNode* src);
};

#endif  // CLIMATE_CHANGE_VISUALISATION_ANIMATION_H_
