#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "animation.h"
#include "assimp_utils.h"

Animation::Animation(const std::string& animation_path, Model& model)
    : duration_(0), ticks_per_second_(0) {
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(animation_path, aiProcess_Triangulate);
  auto animation = scene->mAnimations[0];

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "Failed to load model: " << importer.GetErrorString()
              << std::endl;

    return;
  }

  duration_ = animation->mDuration;
  ticks_per_second_ = animation->mTicksPerSecond;

  ReadHeirarchyData(root_node_, scene->mRootNode);
  ReadMissingBones(animation, model);
}

Bone* Animation::FindBone(const std::string& name) {
  auto iter = std::find_if(bones_.begin(), bones_.end(), [&](const Bone& Bone) {
    return Bone.GetBoneName() == name;
  });

  if (iter == bones_.end()) {
    return nullptr;
  }

  return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model) {
  int size = animation->mNumChannels;

  std::unordered_map<std::string, BoneInfo>& bone_info_map =
      model.bone_info_map_;
  int& bone_count = model.bone_count_;

  for (int i = 0; i < size; i++) {
    auto channel = animation->mChannels[i];
    std::string boneName = channel->mNodeName.data;

    if (bone_info_map.find(boneName) == bone_info_map.end()) {
      bone_info_map[boneName].id = bone_count;
      bone_count++;
    }
    bones_.push_back(Bone(channel->mNodeName.data,
                          bone_info_map[channel->mNodeName.data].id, channel));
  }

  bone_info_map_ = bone_info_map;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, aiNode* src) {
  dest.name = src->mName.data;
  dest.transformation = assimp_utils::ConvertToMat4(src->mTransformation);
  dest.children_count = src->mNumChildren;

  for (int i = 0; i < src->mNumChildren; i++) {
    AssimpNodeData newData;
    ReadHeirarchyData(newData, src->mChildren[i]);

    dest.children.push_back(newData);
  }
}
