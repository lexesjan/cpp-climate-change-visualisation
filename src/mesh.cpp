#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
           std::vector<Texture>& textures, Shader& shader, Renderer& renderer)
    : vertices_(vertices),
      indices_(indices),
      textures_(textures),
      shader_(shader),
      renderer_(renderer) {
  InitMesh();
}

void Mesh::Draw() {}

void Mesh::InitMesh() {
  VertexBufferObject vertex_buffer_object(
      vertices_.data(), vertices_.size() * sizeof(vertices_.front()));

  VertexBufferLayout vertex_buffer_layout;
  vertex_buffer_layout.AddElement<float>(3);
  vertex_buffer_layout.AddElement<float>(3);
  vertex_buffer_layout.AddElement<float>(2);

  vertex_array_object_.AddBuffer(vertex_buffer_object, vertex_buffer_layout);
}
