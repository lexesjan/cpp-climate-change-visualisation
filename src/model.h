#ifndef CLIMATE_CHANGE_VISUALISATION_MODEL_H_
#define CLIMATE_CHANGE_VISUALISATION_MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <set>
#include "shader.h"
#include "mesh.h"

class Model {
 public:
  explicit Model(std::string path, Shader shader, Renderer renderer);
  void Draw() const;

 private:
  std::set<std::string> loaded_textures_;
  std::vector<Mesh> meshes_;
  std::string directory_;
  Shader shader_;
  Renderer renderer_;

  void LoadModel(std::string path);

  void ProcessNode(aiNode *node, const aiScene *scene);

  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<Texture> LoadMaterialTextures(aiMaterial *material,
                                            aiTextureType type,
                                            std::string type_name);
};

#endif
