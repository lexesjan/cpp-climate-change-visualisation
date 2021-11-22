#include <iostream>
#include <stb_image.h>
#include "texture.h"

Texture::Texture(std::string &path) : path_(path), id_(0) {
  stbi_set_flip_vertically_on_load(true);

  int width;
  int height;
  int num_channels;

  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &num_channels, 0);

  if (!data) {
    std::cerr << "Failed to load texture at path: " << path << std::endl;

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

  glGenTextures(1, &id_);
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

GLuint Texture::GetId() const { return id_; }

const std::string &Texture::GetType() const { return type_; }

void Texture::SetType(std::string &type) { type_ = type; }

const std::string &Texture::GetPath() const { return path_; }

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
