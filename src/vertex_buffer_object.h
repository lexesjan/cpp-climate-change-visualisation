#ifndef CLIMATE_CHANGE_VISUALISATION_VERTEX_BUFFER_OBJECT_H_
#define CLIMATE_CHANGE_VISUALISATION_VERTEX_BUFFER_OBJECT_H_

#include <GL/glew.h>

class VertexBufferObject {
 public:
  explicit VertexBufferObject(const void* data, unsigned int size);

  void Bind();

  void Unbind();

 private:
  GLuint id_;
};

#endif
