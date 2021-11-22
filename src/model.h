#ifndef CLIMATE_CHANGE_VISUALISATION_MODEL_H_
#define CLIMATE_CHANGE_VISUALISATION_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include <set>
#include "shader.h"
#include "mesh.h"

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

struct BoneInfo {
  int id;
  glm::mat4 offset;
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

class Model {
 public:
  explicit Model(std::string path, Shader shader, Renderer renderer);

  void UpdateBoneTransformations();

  const std::vector<glm::mat4> &GetFinalTransforms() const;

  void Draw() const;

  void SetDelta(float delta);

 private:
  std::set<std::string> loaded_textures_;
  std::vector<Mesh> meshes_;
  std::string directory_;
  std::vector<glm::mat4> final_transforms_;
  std::unordered_map<std::string, AssimpNodeAnimation> node_animations_map_;
  int bone_count_;
  std::unordered_map<std::string, BoneInfo> bone_info_map_;
  Shader shader_;
  Renderer renderer_;
  float current_time_;
  glm::mat4 global_inverse_transform_;
  float delta_;
  float ticks_per_second_;
  float animation_duration_;
  AssimpNode root_node_;

  void LoadModel(std::string path);

  void ProcessNode(aiNode *node, const aiScene *scene);

  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<Texture> LoadMaterialTextures(aiMaterial *material,
                                            aiTextureType type,
                                            std::string type_name);

  void InitialiseVertexBoneInfo(Vertex &vertex) const;

  void InsertBoneInfo(Vertex &vertex, int bone_id, float weight) const;

  void ExtractBoneInfo(std::vector<Vertex> &vertices, aiMesh *mesh,
                       const aiScene *scene);

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

#endif  // CLIMATE_CHANGE_VISUALISATION_MODEL_H_
