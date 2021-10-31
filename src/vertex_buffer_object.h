#ifndef CLIMATE_CHANGE_VISUALISATION_VERTEX_BUFFER_OBJECT_H_
#define CLIMATE_CHANGE_VISUALISATION_VERTEX_BUFFER_OBJECT_H_

#include <GL/glew.h>

class VertexBufferObject {
 public:
  explicit VertexBufferObject();
  ~VertexBufferObject();

  void Bind();

  void Unbind();

 private:
  GLuint id_;
  GLuint count_;
};

#endif
