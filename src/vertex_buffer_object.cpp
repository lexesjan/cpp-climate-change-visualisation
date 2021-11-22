#include "vertex_buffer_object.h"

VertexBufferObject::VertexBufferObject(const void* data, unsigned int size) {
  glGenBuffers(1, &id_);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferObject::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }

void VertexBufferObject::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
