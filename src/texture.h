#ifndef CLIMATE_CHANGE_VISUALISATION_TEXTURE_H_
#define CLIMATE_CHANGE_VISUALISATION_TEXTURE_H_

#include <GL/glew.h>
#include <string>

class Texture {
 public:
  explicit Texture(std::string& path);

  GLuint GetId() const;

  const std::string& GetType() const;

  void Bind(unsigned int slot = 0) const;

  void Unbind() const;

 private:
  GLuint id_;
  std::string type_;
  std::string path_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_TEXTURE_H_
