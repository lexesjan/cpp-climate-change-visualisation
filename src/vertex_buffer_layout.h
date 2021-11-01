#ifndef CLIMATE_CHANGE_VISUALISATION_VERTEX_BUFFER_LAYOUT_H_
#define CLIMATE_CHANGE_VISUALISATION_VERTEX_BUFFER_LAYOUT_H_

#include <GL/glew.h>
#include <vector>

struct VertexBufferObjectElement {
  GLint count;
  GLenum type;
  GLboolean normalised;
};

class VertexBufferLayout {
 public:
  explicit VertexBufferLayout();

  template <typename T>
  void AddElement(GLint count) {
    static_assert(false);
  }

  template <>
  void AddElement<float>(GLint count) {
    elements_.push_back({count, GL_FLOAT, GL_FALSE});

    stride_ += GetSizeOfType(GL_FLOAT) * count;
  }

  unsigned int GetStride() const;

  const std::vector<VertexBufferObjectElement> &GetElements() const;

  static unsigned int GetSizeOfType(GLenum type);

 private:
  unsigned int stride_;
  std::vector<VertexBufferObjectElement> elements_;
};

#endif
