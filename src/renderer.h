#ifndef CLIMATE_CHANGE_VISUALISATION_RENDERER_H_
#define CLIMATE_CHANGE_VISUALISATION_RENDERER_H_

#include "vertex_array_object.h"
#include "shader.h"
#include "renderable.h"

class Renderer {
 public:
  void Init() const;

  void Clear() const;

  void Draw(const VertexArrayObject &vertex_array_object, const Shader &shader,
            GLsizei count) const;
  void Draw(const Renderable &entity, const Shader &shader);
};

#endif
