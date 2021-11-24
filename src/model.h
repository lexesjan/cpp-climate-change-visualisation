#ifndef CLIMATE_CHANGE_VISUALISATION_MODEL_H_
#define CLIMATE_CHANGE_VISUALISATION_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <set>
#include "shader.h"
#include "mesh.h"
#include "material.h"

struct BoneInfo {
  int id;
  glm::mat4 offset;
};

class Model {
 public:
  explicit Model(std::string path, Shader &shader, Renderer &renderer,
                 Material material = Material(glm::vec3(0.0f), glm::vec3(0.0f),
                                              glm::vec3(0.0f)));

  virtual void Draw();

 protected:
  Shader shader_;
  Renderer renderer_;
  std::unordered_map<std::string, BoneInfo> bone_info_map_;
  unsigned int bone_count_;

  std::vector<Texture> LoadMaterialTextures(aiMaterial *material,
                                            aiTextureType type,
                                            std::string type_name);

 private:
  std::set<std::string> loaded_textures_;
  std::vector<Mesh> meshes_;
  std::string directory_;
  Material material_;

  void LoadModel(std::string path);

  void ProcessNode(aiNode *node, const aiScene *scene);

  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

  void InitialiseVertexBoneInfo(Vertex &vertex) const;

  void InsertBoneInfo(Vertex &vertex, int bone_id, float weight) const;

  void ExtractBoneInfo(std::vector<Vertex> &vertices, aiMesh *mesh,
                       const aiScene *scene);
};

#endif  // CLIMATE_CHANGE_VISUALISATION_MODEL_H_
