#include "vertex_array_object.h"

VertexArrayObject::VertexArrayObject() { glGenVertexArrays(1, &id_); }

void VertexArrayObject::AddBuffer(
    const VertexBufferObject& vertex_buffer_object,
    const VertexBufferLayout& vertex_buffer_layout,
    const ElementBufferObject& element_buffer_object) {
  Bind();
  vertex_buffer_object.Bind();
  element_buffer_object.Bind();

  unsigned int offset = 0;
  const std::vector<VertexBufferObjectElement>& elements =
      vertex_buffer_layout.GetElements();

  for (unsigned int i = 0; i < elements.size(); ++i) {
    const VertexBufferObjectElement& element = elements[i];

    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalised,
                          vertex_buffer_layout.GetStride(),
                          (const void*)offset);

    offset += element.count * VertexBufferLayout::GetSizeOfType(element.type);
  }
}

void VertexArrayObject::Bind() const { glBindVertexArray(id_); }

void VertexArrayObject::Unbind() const { glBindVertexArray(0); }
