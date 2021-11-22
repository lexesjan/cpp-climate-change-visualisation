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
  explicit Mesh(const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices,
                const std::vector<Texture>& textures, const Shader& shader,
                const Renderer& renderer);

  void Draw() const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;
  Shader shader_;
  VertexArrayObject vertex_array_object_;
  ElementBufferObject element_buffer_object_;
  Renderer renderer_;

  void InitMesh();
};

#endif  // CLIMATE_CHANGE_VISUALISATION_MESH_H_
