#include <iostream>
#include "model.h"

Model::Model(std::string path, Shader shader, Renderer renderer)
    : shader_(shader), renderer_(renderer) {
  LoadModel(path);
}

void Model::Draw() const {
  for (const Mesh& mesh : meshes_) {
    mesh.Draw();
  }
}

void Model::LoadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "Failed to load model: " << importer.GetErrorString()
              << std::endl;

    return;
  }

  std::string directory = path.substr(0, path.find_last_of('/'));

  if (directory.find_last_of('/') == directory.npos ||
      directory.find_last_of('/') != directory.size() - 1) {
    directory += "/";
  }

  directory_ = directory;

  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

    meshes_.push_back(ProcessMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    if (mesh->HasPositions()) {
      const aiVector3D* position = &(mesh->mVertices[i]);

      vertex.position = glm::vec3(position->x, position->y, position->z);
    }

    if (mesh->HasNormals()) {
      const aiVector3D* normal = &(mesh->mNormals[i]);

      vertex.normal = glm::vec3(normal->x, normal->y, normal->z);
    }

    if (mesh->HasTextureCoords(0)) {
      const aiVector3D* texture_coord = &(mesh->mTextureCoords[0][i]);

      vertex.texture_coords = glm::vec2(texture_coord->x, texture_coord->y);
    } else {
      vertex.texture_coords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuse_maps = LoadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    std::vector<Texture> specular_maps = LoadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    std::vector<Texture> normal_maps = LoadMaterialTextures(
        material, aiTextureType_NORMALS, "texture_normals");
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
  }

  return Mesh(vertices, indices, textures, shader_, renderer_);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material,
                                                 aiTextureType type,
                                                 std::string type_name) {
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;

    material->GetTexture(type, i, &str);

    std::string path = directory_ + std::string(str.C_Str());

    if (loaded_textures_.find(path) != loaded_textures_.end()) {
      continue;
    }

    loaded_textures_.insert(path);

    Texture texture(path);
    texture.SetType(type_name);

    textures.push_back(texture);
  }

  return textures;
}
