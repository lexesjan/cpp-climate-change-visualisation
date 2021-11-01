#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "mesh.h"

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

void Renderer::Draw(const Mesh& mesh, Shader& shader) const {
  shader.Bind();
  mesh.GetVertexArrayObject().Bind();

  glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
}

void Renderer::Draw(const std::vector<Mesh>& meshes, Shader& shader) const {
  shader.Bind();

  for (unsigned int i = 0; i < meshes.size(); ++i) {
    const Mesh& mesh = meshes[i];

    shader.SetUniformMatrix4fv("model", GL_FALSE,
                               glm::value_ptr(mesh.GetModelMatrix()));

    mesh.GetVertexArrayObject().Bind();

    glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
  }
}
