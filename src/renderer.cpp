#include "renderer.h"
#include "mesh.h"

void Renderer::Init() const {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::Clear() const {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArrayObject& vertex_array_object,
                    const Shader& shader, GLsizei count) const {
  shader.Bind();
  vertex_array_object.Bind();

  glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::Draw(const VertexArrayObject& vertex_array_object,
                    const ElementBufferObject& element_buffer_object,
                    const Shader& shader, GLsizei count) const {
  shader.Bind();
  vertex_array_object.Bind();
  element_buffer_object.Bind();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

  shader.Unbind();
  vertex_array_object.Unbind();
  element_buffer_object.Unbind();
}
