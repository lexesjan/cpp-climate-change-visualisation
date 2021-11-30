#ifndef CLIMATE_CHANGE_VISUALISATION_SKYBOX_H_
#define CLIMATE_CHANGE_VISUALISATION_SKYBOX_H_

#include <GL/glew.h>
#include <vector>
#include <string>
#include "vertex_array_object.h"
#include "shader.h"
#include "renderer.h"

class Skybox {
 public:
  explicit Skybox(std::vector<std::string> faces, Shader shader,
                  Renderer renderer);

  void Draw();

 private:
  GLuint texture_id_;
  Shader shader_;
  Renderer renderer_;
  VertexArrayObject vertex_array_object_;
  std::vector<glm::vec3> vertices_;

  void Bind() const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_SKYBOX_H_