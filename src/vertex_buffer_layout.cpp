#include "vertex_buffer_layout.h"

VertexBufferLayout::VertexBufferLayout() : stride_(0), elements_() {}

unsigned int VertexBufferLayout::GetStride() const { return stride_; }

const std::vector<VertexBufferObjectElement> &VertexBufferLayout::GetElements()
    const {
  return elements_;
}

unsigned int VertexBufferLayout::GetSizeOfType(GLenum type) {
  switch (type) {
    case GL_FLOAT:
      return sizeof(GLfloat);
    case GL_INT:
      return sizeof(GLint);
  }

  return 0;
}
