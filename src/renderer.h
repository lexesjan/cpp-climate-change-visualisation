#ifndef CLIMATE_CHANGE_VISUALISATION_RENDERER_H_
#define CLIMATE_CHANGE_VISUALISATION_RENDERER_H_

#include "mesh.h"
#include "vertex_array_object.h"
#include "shader.h"

class Renderer {
 public:
  void Init() const;

  void Clear() const;

  void Draw(const VertexArrayObject &vertex_array_object, const Shader &shader,
            GLsizei count) const;
  void Draw(const Mesh &mesh, Shader &shader) const;
  void Draw(const std::vector<Mesh> &meshes, Shader &shader) const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_RENDERER_H_
