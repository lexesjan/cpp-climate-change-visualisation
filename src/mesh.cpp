#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures, const Material& material,
           const Shader& shader, const Renderer& renderer)
    : vertices_(vertices),
      indices_(indices),
      textures_(textures),
      material_(material),
      shader_(shader),
      renderer_(renderer) {
  InitMesh();
}

void Mesh::Draw() {
  unsigned int num_diffuse = 1;
  unsigned int num_specular = 1;
  unsigned int num_normals = 1;

  for (unsigned int i = 0; i < textures_.size(); i++) {
    const Texture& texture = textures_[i];
    texture.Bind(i);

    std::string number;
    std::string type = texture.GetType();

    if (type == "texture_diffuse") {
      number = std::to_string(num_diffuse++);
    } else if (type == "texture_specular") {
      number = std::to_string(num_specular++);
    } else if (type == "texture_normals") {
      number = std::to_string(num_normals++);
    }

    shader_.SetUniform1i("material." + type + number, i);
  }

  shader_.SetUniform1f("material.shininess", material_.shininess);

  renderer_.Draw(vertex_array_object_, element_buffer_object_, shader_,
                 (GLsizei)indices_.size());

  for (unsigned int i = 0; i < textures_.size(); i++) {
    const Texture& texture = textures_[i];
    texture.Unbind(i);
  }
}

void Mesh::InitMesh() {
  VertexBufferObject vertex_buffer_object(
      vertices_.data(),
      (GLsizei)(vertices_.size() * sizeof(vertices_.front())));

  VertexBufferLayout vertex_buffer_layout;
  vertex_buffer_layout.AddElement<float>(3);
  vertex_buffer_layout.AddElement<float>(3);
  vertex_buffer_layout.AddElement<float>(2);
  vertex_buffer_layout.AddElement<int>(4);
  vertex_buffer_layout.AddElement<float>(4);

  element_buffer_object_ =
      ElementBufferObject(indices_.data(), (unsigned int)indices_.size());

  vertex_array_object_.AddBuffer(vertex_buffer_object, vertex_buffer_layout);
}
