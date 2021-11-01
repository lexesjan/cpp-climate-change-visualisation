#ifndef CLIMATE_CHANGE_VISUALISATION_RENDERABLE_H_
#define CLIMATE_CHANGE_VISUALISATION_RENDERABLE_H_

#include "vertex_array_object.h"

class Renderable {
 public:
  virtual const VertexArrayObject &GetVertexArrayObject() const = 0;

  virtual const GLsizei GetVertexCount() const = 0;
};

#endif
