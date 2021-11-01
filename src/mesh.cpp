#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>
#include <iostream>
#include "mesh.h"

Mesh::Mesh(const std::string& file_path) {
  model_ = glm::mat4(1.0f);
  LoadMesh(file_path);
  InitMesh();
}

const VertexArrayObject& Mesh::GetVertexArrayObject() const {
  return vertex_array_object_;
}

const GLsizei Mesh::GetVertexCount() const { return vertices.size(); }

const glm::mat4& Mesh::GetModelMatrix() const { return model_; }

void Mesh::SetModelMatrix(const glm::mat4& model) { model_ = model; }

void Mesh::LoadMesh(const std::string& file_path) {
  const aiScene* scene =
      aiImportFile(file_path.c_str(),
                   aiProcess_Triangulate | aiProcess_PreTransformVertices);

  if (!scene) {
    std::cerr << "ERROR: reading mesh: " << file_path << std::endl;
    exit(1);
  }

  std::cout << scene->mNumMaterials << " materials" << std::endl;
  std::cout << scene->mNumMeshes << " meshes" << std::endl;
  std::cout << scene->mNumTextures << " textures" << std::endl;

  for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
    const aiMesh* mesh = scene->mMeshes[i];

    std::cout << mesh->mNumVertices << " vertices in mesh" << std::endl;

    for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
      Vertex vertex;

      if (mesh->HasPositions()) {
        const aiVector3D* position = &(mesh->mVertices[j]);
        vertex.position = glm::vec3(position->x, position->y, position->z);
      }
      if (mesh->HasNormals()) {
        const aiVector3D* normal = &(mesh->mNormals[j]);
        vertex.normal = glm::vec3(normal->x, normal->y, normal->z);
      }
      if (mesh->HasTextureCoords(0)) {
        const aiVector3D* texture_coord = &(mesh->mTextureCoords[0][j]);
        vertex.texture_coords = glm::vec2(texture_coord->x, texture_coord->y);
      }

      vertices.push_back(vertex);
    }
  }

  aiReleaseImport(scene);
}

void Mesh::InitMesh() {
  VertexBufferObject vertex_buffer_object(
      vertices.data(), vertices.size() * sizeof(vertices.front()));

  VertexBufferLayout layout;
  layout.AddElement<float>(3);
  layout.AddElement<float>(3);
  layout.AddElement<float>(2);

  vertex_array_object_.AddBuffer(vertex_buffer_object, layout);
}
