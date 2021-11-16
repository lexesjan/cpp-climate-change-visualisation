#ifndef CLIMATE_CHANGE_VISUALISATION_ELEMENT_BUFFER_OBJECT_H_
#define CLIMATE_CHANGE_VISUALISATION_ELEMENT_BUFFER_OBJECT_H_

#include <GL/glew.h>

class ElementBufferObject {
 public:
  explicit ElementBufferObject(const void* data, unsigned int count);

  void Bind() const;

  void Unbind() const;

 private:
  GLuint id_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_ELEMENT_BUFFER_OBJECT_H_
