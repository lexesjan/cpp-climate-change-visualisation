#ifndef CLIMATE_CHANGE_VISUALISATION_MODEL_H_
#define CLIMATE_CHANGE_VISUALISATION_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <set>
#include "shader.h"
#include "mesh.h"

struct BoneInfo {
  int id;
  glm::mat4 offset;
};

class Model {
 public:
  explicit Model(std::string path, Shader shader, Renderer renderer);

  void UpdateBoneTransformations(float delta);

  const std::vector<glm::mat4> &GetFinalTransforms() const;

  void Draw() const;

 private:
  Assimp::Importer importer_;
  const aiScene *scene_;
  std::set<std::string> loaded_textures_;
  std::vector<Mesh> meshes_;
  std::string directory_;
  std::vector<glm::mat4> final_transforms_;
  std::unordered_map<std::string, aiNodeAnim *> node_animations_map_;
  int bone_count_;
  std::unordered_map<std::string, BoneInfo> bone_info_map_;
  Shader shader_;
  Renderer renderer_;
  float current_time_;

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

  void ReadNodeHeirarchy(float animation_time, aiNode *node,
                         glm::mat4 parent_transform);

  const glm::mat4 &InterpolateScaling(float animation_time,
                                      aiNodeAnim *node_animation) const;

  const glm::mat4 &InterpolateRotation(float animation_time,
                                       aiNodeAnim *node_animation) const;

  const glm::mat4 &InterpolateTranslation(float animation_time,
                                          aiNodeAnim *node_animation) const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_MODEL_H_
