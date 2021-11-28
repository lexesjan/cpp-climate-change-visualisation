#include <stb_image.h>
#include <iostream>
#include "skybox.h"

Skybox::Skybox(std::vector<std::string> faces, Shader shader, Renderer renderer)
    : texture_id_(0),
      shader_(shader),
      renderer_(renderer),
      vertex_array_object_(),
      vertices_(
          {{-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
           {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},

           {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},
           {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

           {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},

           {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

           {-1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},

           {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, -1.0f},
           {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f}}) {
  stbi_set_flip_vertically_on_load(false);

  glGenTextures(1, &texture_id_);
  Bind();

  int width;
  int height;
  int num_channels;

  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char* data =
        stbi_load(faces[i].c_str(), &width, &height, &num_channels, 0);

    if (!data) {
      std::cerr << "Failed to load texture at path: " << faces[i] << std::endl;

      return;
    }

    GLenum format = 0;
    switch (num_channels) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  VertexBufferObject vertex_buffer_object(
      vertices_.data(), (GLsizei)vertices_.size() * sizeof(vertices_.front()));

  VertexBufferLayout vertex_buffer_layout;
  vertex_buffer_layout.AddElement<float>(3);

  vertex_array_object_.AddBuffer(vertex_buffer_object, vertex_buffer_layout);
}

void Skybox::Draw() {
  glDepthMask(GL_FALSE);
  Bind();
  shader_.SetUniform1i("skybox", 0);
  renderer_.Draw(vertex_array_object_, shader_, (GLsizei)vertices_.size());
  glDepthMask(GL_TRUE);
}

void Skybox::Bind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
}
