#ifndef CLIMATE_CHANGE_VISUALISATION_MESH_H_
#define CLIMATE_CHANGE_VISUALISATION_MESH_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include "renderer.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coords;
};

struct Texture {
  unsigned int id;
  std::string type;
};

class Mesh {
 public:
  explicit Mesh(const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices,
                const std::vector<Texture>& textures, const Shader& shader,
                const Renderer& renderer);
  void Draw();

 private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;
  Shader shader_;
  VertexArrayObject vertex_array_object_;
  Renderer renderer_;

  void InitMesh();
};

#endif  // CLIMATE_CHANGE_VISUALISATION_MESH_H_
