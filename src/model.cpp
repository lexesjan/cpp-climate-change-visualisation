#include <iostream>
#include "model.h"
#include "assimp_utils.h"

Model::Model(std::string path, Shader shader, Renderer renderer)
    : shader_(shader), renderer_(renderer), bone_count_(0), texture_(nullptr) {
  LoadModel(path);
}

Model::Model(std::string path, Shader shader, Renderer renderer,
             Texture* texture)
    : shader_(shader), renderer_(renderer), bone_count_(0), texture_(texture) {
  LoadModel(path);
}

void Model::Draw() const {
  shader_.Bind();

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
  Material loaded_material;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    InitialiseVertexBoneInfo(vertex);

    vertex.position = assimp_utils::ConvertToVec3(mesh->mVertices[i]);
    vertex.normal = assimp_utils::ConvertToVec3(mesh->mNormals[i]);

    if (mesh->HasTextureCoords(0)) {
      vertex.texture_coords =
          assimp_utils::ConvertToVec2(mesh->mTextureCoords[0][i]);
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

    loaded_material = LoadMaterial(material);
  }

  if (textures.empty() && texture_) {
    textures.push_back(*texture_);
  }

  ExtractBoneInfo(vertices, mesh, scene);

  return Mesh(vertices, indices, textures, loaded_material, shader_, renderer_);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material,
                                                 aiTextureType type,
                                                 std::string type_name) {
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;

    material->GetTexture(type, i, &str);

    std::string path = directory_ + str.C_Str();

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

Material Model::LoadMaterial(aiMaterial* material) {
  Material result;

  aiVector3D ambient(0.0f);
  material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
  result.ambient = assimp_utils::ConvertToVec3(ambient);

  aiVector3D diffuse(0.0f);
  material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
  result.diffuse = assimp_utils::ConvertToVec3(diffuse);

  aiVector3D specular(0.0f);
  material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
  result.specular = assimp_utils::ConvertToVec3(specular);

  float shininess = 0.0f;
  material->Get(AI_MATKEY_SHININESS, shininess);
  result.shininess = shininess;

  return result;
}

void Model::InitialiseVertexBoneInfo(Vertex& vertex) const {
  for (unsigned int i = 0; i < (unsigned int)vertex.bone_ids.length(); i++) {
    vertex.bone_ids[i] = 0;
    vertex.weights[i] = 0.0f;
  }
}

void Model::InsertBoneInfo(Vertex& vertex, int bone_id, float weight) const {
  for (unsigned int i = 0; i < (unsigned int)vertex.bone_ids.length(); i++) {
    if (vertex.weights[i] == 0.0f) {
      vertex.bone_ids[i] = bone_id;
      vertex.weights[i] = weight;

      return;
    }
  }
}

void Model::ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh,
                            const aiScene* scene) {
  for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    int bone_id = -1;

    aiBone* bone = mesh->mBones[i];

    std::string bone_name = bone->mName.C_Str();

    if (bone_info_map_.find(bone_name) == bone_info_map_.end()) {
      BoneInfo bone_info;
      bone_info.id = bone_count_;
      bone_info.offset = assimp_utils::ConvertToMat4(bone->mOffsetMatrix);

      bone_info_map_[bone_name] = bone_info;

      bone_id = bone_count_;

      bone_count_++;
    } else {
      bone_id = bone_info_map_[bone_name].id;
    }

    aiVertexWeight* weights = bone->mWeights;
    unsigned int num_weights = bone->mNumWeights;

    for (unsigned int j = 0; j < num_weights; j++) {
      int vertex_id = weights[j].mVertexId;
      float weight = weights[j].mWeight;

      InsertBoneInfo(vertices[vertex_id], bone_id, weight);
    }
  }
}
