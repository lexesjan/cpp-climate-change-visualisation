#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures, const Shader& shader,
           const Renderer& renderer)
    : vertices_(vertices),
      indices_(indices),
      textures_(textures),
      shader_(shader),
      renderer_(renderer) {
  InitMesh();
}

void Mesh::Draw() {
  renderer_.Draw(vertex_array_object_, shader_, indices_.size());
}

void Mesh::InitMesh() {
  VertexBufferObject vertex_buffer_object(
      vertices_.data(), vertices_.size() * sizeof(vertices_.front()));

  VertexBufferLayout vertex_buffer_layout;
  vertex_buffer_layout.AddElement<float>(3);
  vertex_buffer_layout.AddElement<float>(3);
  vertex_buffer_layout.AddElement<float>(2);

  ElementBufferObject element_buffer_object(indices_.data(), indices_.size());

  vertex_array_object_.AddBuffer(vertex_buffer_object, vertex_buffer_layout,
                                 element_buffer_object);
}
