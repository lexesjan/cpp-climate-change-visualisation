#ifndef CLIMATE_CHANGE_VISUALISATION_MESH_H_
#define CLIMATE_CHANGE_VISUALISATION_MESH_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int4.hpp>
#include <glm/ext/vector_float4.hpp>
#include "renderer.h"
#include "texture.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coords;
  glm::ivec4 bone_ids;
  glm::vec4 weights;
};

class Mesh {
 public:
  explicit Mesh(std::vector<Vertex> &vertices,
                std::vector<unsigned int> &indices,
                std::vector<Texture> &textures, Shader &shader,
                Renderer &renderer);

  void Draw();

  void SetUseTexture(bool use_texture);

 private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;
  Shader shader_;
  VertexArrayObject vertex_array_object_;
  ElementBufferObject element_buffer_object_;
  Renderer renderer_;
  bool use_texture_;

  void InitMesh();
};

#endif  // CLIMATE_CHANGE_VISUALISATION_MESH_H_
