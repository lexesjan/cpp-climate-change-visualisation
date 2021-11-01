#include "renderer.h"

void Renderer::Init() const {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void Renderer::Clear() const {
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArrayObject& vertex_array_object,
                    const Shader& shader, GLsizei count) const {
  shader.Bind();
  vertex_array_object.Bind();

  glDrawArrays(GL_TRIANGLES, 0, count);
}
