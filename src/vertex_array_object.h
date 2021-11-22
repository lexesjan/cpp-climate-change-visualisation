#ifndef CLIMATE_CHANGE_VISUALISATION_VERTEX_ARRAY_OBJECT_H_
#define CLIMATE_CHANGE_VISUALISATION_VERTEX_ARRAY_OBJECT_H_

#include <GL/glew.h>
#include "vertex_buffer_layout.h"
#include "vertex_buffer_object.h"
#include "element_buffer_object.h"

class VertexArrayObject {
 public:
  explicit VertexArrayObject();

  void AddBuffer(const VertexBufferObject& vertex_buffer_object,
                 const VertexBufferLayout& vertex_buffer_layout);

  void Bind() const;

  void Unbind() const;

 private:
  GLuint id_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_VERTEX_ARRAY_OBJECT_H_
