#include <iostream>
#include "model.h"
#include "assimp_utils.h"

Model::Model(std::string path, Shader shader, Renderer renderer)
    : shader_(shader),
      renderer_(renderer),
      bone_count_(0),
      current_time_(0.0f),
      delta_(0.0f),
      ticks_per_second_(0.0f),
      animation_duration_(0.0f) {
  LoadModel(path);
}

void Model::Draw() const {
  shader_.Bind();

  for (const Mesh& mesh : meshes_) {
    mesh.Draw();
  }
}

void Model::SetDelta(float delta) { delta_ = delta; }

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

  if (scene->HasAnimations()) {
    aiAnimation* animation = scene->mAnimations[0];
    ticks_per_second_ = (float)animation->mTicksPerSecond;
    animation_duration_ = (float)animation->mDuration;

    InitialiseNodeAnimationsMap(animation);
  }

  global_inverse_transform_ = glm::mat4(1.0f);

  ProcessNode(scene->mRootNode, scene);

  ReadHeirarchyData(root_node_, scene->mRootNode);

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

void Model::ReadHeirarchyData(AssimpNode& destination, aiNode* source) {
  destination.name = source->mName.data;
  destination.transformation =
      assimp_utils::ConvertToMat4(source->mTransformation);
  destination.children_count = source->mNumChildren;

  for (unsigned int i = 0; i < source->mNumChildren; i++) {
    AssimpNode newData;
    ReadHeirarchyData(newData, source->mChildren[i]);

    destination.children.push_back(newData);
  }
}

void Model::UpdateBoneTransformations() {
  if (node_animations_map_.empty()) {
    return;
  }

  current_time_ += ticks_per_second_ * delta_;
  current_time_ = (float)fmod(current_time_, animation_duration_);

  ProcessNodeHeirarchy(current_time_, root_node_, glm::mat4(1.0f));
}

const std::vector<glm::mat4>& Model::GetFinalTransforms() const {
  return final_transforms_;
}

void Model::ProcessNodeHeirarchy(float animation_time, AssimpNode& node,
                                 glm::mat4 parent_transform) {
  std::string node_name = node.name;
  glm::mat4 node_transform = node.transformation;

  if (node_animations_map_.find(node_name) != node_animations_map_.end()) {
    AssimpNodeAnimation node_animation = node_animations_map_[node_name];

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

  for (unsigned int i = 0; i < node.children_count; i++) {
    ProcessNodeHeirarchy(animation_time, node.children[i], global_transform);
  }
}

void Model::InitialiseNodeAnimationsMap(aiAnimation* animation) {
  for (unsigned int i = 0; i < animation->mNumChannels; i++) {
    aiNodeAnim* node_animation = animation->mChannels[i];
    std::string node_name = node_animation->mNodeName.C_Str();

    AssimpNodeAnimation converted_node_animation;

    for (unsigned int i = 0; i < node_animation->mNumPositionKeys; i++) {
      KeyPosition key_position;
      key_position.position =
          assimp_utils::ConvertToVec3(node_animation->mPositionKeys[i].mValue);
      key_position.time_stamp = (float)node_animation->mPositionKeys[i].mTime;

      converted_node_animation.key_positions.push_back(key_position);
    }

    for (unsigned int i = 0; i < node_animation->mNumRotationKeys; i++) {
      KeyRotation key_rotation;
      key_rotation.orientation =
          assimp_utils::ConvertToQuat(node_animation->mRotationKeys[i].mValue);
      key_rotation.time_stamp = (float)node_animation->mRotationKeys[i].mTime;

      converted_node_animation.key_rotations.push_back(key_rotation);
    }

    for (unsigned int i = 0; i < node_animation->mNumScalingKeys; i++) {
      KeyScale key_scale;
      key_scale.scale =
          assimp_utils::ConvertToVec3(node_animation->mScalingKeys[i].mValue);
      key_scale.time_stamp = (float)node_animation->mScalingKeys[i].mTime;

      converted_node_animation.key_scales.push_back(key_scale);
    }

    node_animations_map_[node_name] = converted_node_animation;
  }
}

float Model::GetScalingFactor(float last_time_stamp, float next_time_stamp,
                              float animation_time) const {
  float mid_way_length = animation_time - last_time_stamp;
  float frames_difference = next_time_stamp - last_time_stamp;

  return mid_way_length / frames_difference;
}

const glm::mat4 Model::InterpolateScaling(
    float animation_time, AssimpNodeAnimation& node_animation) const {
  if (node_animation.key_scales.size() == 1) {
    return glm::scale(glm::mat4(1.0f), node_animation.key_scales[0].scale);
  }

  unsigned int scaling_index = 0;

  for (unsigned int i = 0; i < node_animation.key_scales.size() - 1; i++) {
    if (animation_time < node_animation.key_scales[i + 1].time_stamp) {
      scaling_index = i;

      break;
    }
  }

  unsigned int next_scaling_index = scaling_index + 1;

  float factor = GetScalingFactor(
      node_animation.key_scales[scaling_index].time_stamp,
      node_animation.key_scales[next_scaling_index].time_stamp, animation_time);

  glm::vec3 start = node_animation.key_scales[scaling_index].scale;
  glm::vec3 end = node_animation.key_scales[next_scaling_index].scale;
  glm::vec3 delta = end - start;
  glm::vec3 scale = start + factor * delta;

  return glm::scale(glm::mat4(1.0f), scale);
}

const glm::mat4 Model::InterpolateRotation(
    float animation_time, AssimpNodeAnimation& node_animation) const {
  if (node_animation.key_rotations.size() == 1) {
    return glm::toMat4(node_animation.key_rotations[0].orientation);
  }

  unsigned int rotation_index = 0;

  for (unsigned int i = 0; i < node_animation.key_rotations.size() - 1; i++) {
    if (animation_time <
        (float)node_animation.key_rotations[i + 1].time_stamp) {
      rotation_index = i;

      break;
    }
  }

  unsigned int next_rotation_index = rotation_index + 1;

  float factor = GetScalingFactor(
      node_animation.key_rotations[rotation_index].time_stamp,
      node_animation.key_rotations[next_rotation_index].time_stamp,
      animation_time);

  glm::quat start_rotation =
      node_animation.key_rotations[rotation_index].orientation;
  glm::quat end_rotation =
      node_animation.key_rotations[next_rotation_index].orientation;
  glm::quat result = glm::slerp(start_rotation, end_rotation, factor);
  result = glm::normalize(result);

  return glm::toMat4(result);
}

const glm::mat4 Model::InterpolateTranslation(
    float animation_time, AssimpNodeAnimation& node_animation) const {
  if (node_animation.key_positions.size() == 1) {
    return glm::translate(glm::mat4(1.0f),
                          node_animation.key_positions[0].position);
  }

  unsigned int position_index = 0;

  for (unsigned int i = 0; i < node_animation.key_positions.size() - 1; i++) {
    if (animation_time < node_animation.key_positions[i + 1].time_stamp) {
      position_index = i;

      break;
    }
  }

  unsigned int next_position_index = position_index + 1;

  float factor = GetScalingFactor(
      node_animation.key_positions[position_index].time_stamp,
      node_animation.key_positions[next_position_index].time_stamp,
      animation_time);

  glm::vec3 start = node_animation.key_positions[position_index].position;
  glm::vec3 end = node_animation.key_positions[next_position_index].position;
  glm::vec3 delta = end - start;

  glm::vec3 result = start + factor * delta;

  return glm::translate(glm::mat4(1.0f), result);
}
