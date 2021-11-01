#include <GL/glew.h>
#include <vector>
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
  }

  return 0;
}
