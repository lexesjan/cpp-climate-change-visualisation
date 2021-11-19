#include <iostream>
#include "model.h"
#include "assimp_utils.h"

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
  scene_ = importer_.ReadFile(path, aiProcess_Triangulate);

  if (!scene_ || scene_->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene_->mRootNode) {
    std::cerr << "Failed to load model: " << importer_.GetErrorString()
              << std::endl;

    return;
  }

  std::string directory = path.substr(0, path.find_last_of('/'));

  if (directory.find_last_of('/') == directory.npos ||
      directory.find_last_of('/') != directory.size() - 1) {
    directory += "/";
  }

  directory_ = directory;

  // Initialise node animation map.
  if (scene_->HasAnimations()) {
    aiAnimation* animation = scene_->mAnimations[0];

    for (unsigned int i = 0; i < animation->mNumChannels; i++) {
      std::string node_name = animation->mChannels[i]->mNodeName.C_Str();

      node_animations_map_[node_name] = animation->mChannels[i];
    }
  }

  global_inverse_transform_ = glm::mat4(1.0f);
  // assimp_utils::ConvertToMat4(scene_->mRootNode->mTransformation.Inverse());

  ProcessNode(scene_->mRootNode, scene_);

  final_transforms_.resize(bone_count_);
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
  }

  ExtractBoneInfo(vertices, mesh, scene);

  return Mesh(vertices, indices, textures, shader_, renderer_);
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

void Model::UpdateBoneTransformations(float delta) {
  if (!scene_->HasAnimations()) {
    return;
  }

  float ticks_per_second = scene_->mAnimations[0]->mTicksPerSecond;
  current_time_ += ticks_per_second * delta;
  current_time_ = fmod(current_time_, scene_->mAnimations[0]->mDuration);

  ReadNodeHeirarchy(current_time_, scene_->mRootNode, glm::mat4(1.0f));
}

const std::vector<glm::mat4>& Model::GetFinalTransforms() const {
  return final_transforms_;
}

void Model::ReadNodeHeirarchy(float animation_time, aiNode* node,
                              glm::mat4 parent_transform) {
  std::string node_name = node->mName.C_Str();

  aiAnimation* animation = scene_->mAnimations[0];

  glm::mat4 node_transform = assimp_utils::ConvertToMat4(node->mTransformation);

  if (node_animations_map_.find(node_name) != node_animations_map_.end()) {
    aiNodeAnim* node_animation = node_animations_map_[node_name];

    glm::mat4 scaling = InterpolateScaling(animation_time, node_animation);
    glm::mat4 rotation = InterpolateRotation(animation_time, node_animation);
    glm::mat4 translation =
        InterpolateTranslation(animation_time, node_animation);

    node_transform = translation * rotation * scaling;
  }

  glm::mat4 global_transform = parent_transform * node_transform;

  if (bone_info_map_.find(node_name) != bone_info_map_.end()) {
    BoneInfo bone_info = bone_info_map_[node_name];
    final_transforms_[bone_info.id] =
        global_inverse_transform_ * global_transform * bone_info.offset;
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ReadNodeHeirarchy(animation_time, node->mChildren[i], global_transform);
  }
}

float Model::GetScalingFactor(float last_time_stamp, float next_time_stamp,
                              float animation_time) const {
  float mid_way_length = animation_time - last_time_stamp;
  float frames_difference = next_time_stamp - last_time_stamp;

  return mid_way_length / frames_difference;
}

const glm::mat4& Model::InterpolateScaling(float animation_time,
                                           aiNodeAnim* node_animation) const {
  if (node_animation->mNumScalingKeys == 1) {
    return glm::scale(
        glm::mat4(1.0f),
        assimp_utils::ConvertToVec3(node_animation->mScalingKeys[0].mValue));
  }

  unsigned int scaling_index = 0;

  for (unsigned int i = 0; i < node_animation->mNumScalingKeys - 1; i++) {
    if (animation_time < (float)node_animation->mScalingKeys[i + 1].mTime) {
      scaling_index = i;

      break;
    }
  }

  unsigned int next_scaling_index = scaling_index + 1;

  float factor = GetScalingFactor(
      node_animation->mScalingKeys[scaling_index].mTime,
      node_animation->mScalingKeys[next_scaling_index].mTime, animation_time);

  aiVector3D start = node_animation->mScalingKeys[scaling_index].mValue;
  aiVector3D end = node_animation->mScalingKeys[next_scaling_index].mValue;
  aiVector3D delta = end - start;
  aiVector3D scale = start + factor * delta;

  return glm::scale(glm::mat4(1.0f), assimp_utils::ConvertToVec3(scale));
}

const glm::mat4& Model::InterpolateRotation(float animation_time,
                                            aiNodeAnim* node_animation) const {
  if (node_animation->mNumRotationKeys == 1) {
    return glm::toMat4(
        assimp_utils::ConvertToQuat(node_animation->mRotationKeys[0].mValue));
  }

  unsigned int rotation_index = 0;

  for (unsigned int i = 0; i < node_animation->mNumRotationKeys - 1; i++) {
    if (animation_time < (float)node_animation->mRotationKeys[i + 1].mTime) {
      rotation_index = i;

      break;
    }
  }

  unsigned int next_rotation_index = rotation_index + 1;

  float factor = GetScalingFactor(
      node_animation->mRotationKeys[rotation_index].mTime,
      node_animation->mRotationKeys[next_rotation_index].mTime, animation_time);

  const aiQuaternion& start_rotation =
      node_animation->mRotationKeys[rotation_index].mValue;
  const aiQuaternion& end_rotation =
      node_animation->mRotationKeys[next_rotation_index].mValue;
  aiQuaternion result;
  aiQuaternion::Interpolate(result, start_rotation, end_rotation, factor);
  result = result.Normalize();

  return glm::toMat4(assimp_utils::ConvertToQuat(result));
}

const glm::mat4& Model::InterpolateTranslation(
    float animation_time, aiNodeAnim* node_animation) const {
  if (node_animation->mNumPositionKeys == 1) {
    return glm::translate(
        glm::mat4(1.0f),
        assimp_utils::ConvertToVec3(node_animation->mPositionKeys[0].mValue));
  }

  unsigned int position_index = 0;

  for (unsigned int i = 0; i < node_animation->mNumPositionKeys - 1; i++) {
    if (animation_time < (float)node_animation->mPositionKeys[i + 1].mTime) {
      position_index = i;

      break;
    }
  }

  unsigned int next_position_index = position_index + 1;

  float factor = GetScalingFactor(
      node_animation->mPositionKeys[position_index].mTime,
      node_animation->mPositionKeys[next_position_index].mTime, animation_time);

  const aiVector3D& start =
      node_animation->mPositionKeys[position_index].mValue;
  const aiVector3D& end =
      node_animation->mPositionKeys[next_position_index].mValue;
  aiVector3D delta = end - start;

  aiVector3D result = start + factor * delta;

  return glm::translate(glm::mat4(1.0f), assimp_utils::ConvertToVec3(result));
}
