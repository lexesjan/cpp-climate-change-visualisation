#ifndef CLIMATE_CHANGE_VISUALISATION_MESH_H_
#define CLIMATE_CHANGE_VISUALISATION_MESH_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include "vertex_array_object.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coords;
};

class Mesh {
 public:
  explicit Mesh(const std::string &file_path);

  const VertexArrayObject &GetVertexArrayObject() const;

  const GLsizei GetVertexCount() const;

  const glm::mat4 &GetModelMatrix() const;

  void SetModelMatrix(const glm::mat4 &model);

 private:
  glm::mat4 model_;
  std::vector<Vertex> vertices;
  VertexArrayObject vertex_array_object_;

  void LoadMesh(const std::string &file_path);

  void InitMesh();
};

#endif
