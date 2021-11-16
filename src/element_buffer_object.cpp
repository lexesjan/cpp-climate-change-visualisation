#include "element_buffer_object.h"

ElementBufferObject::ElementBufferObject(const void* data, unsigned int size) {
  glGenBuffers(1, &id_);
  Bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void ElementBufferObject::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void ElementBufferObject::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
