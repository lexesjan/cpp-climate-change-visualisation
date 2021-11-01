#ifndef CLIMATE_CHANGE_VISUALISATION_SHADER_H_
#define CLIMATE_CHANGE_VISUALISATION_SHADER_H_

#include <GL/glew.h>
#include <string>

class Shader {
 public:
  explicit Shader();
  explicit Shader(const std::string& vetex_shader_file_path,
                  const std::string& fragment_shader_file_path);

  void Bind() const;

  void Unbind() const;

  void SetUniformMatrix4fv(const std::string& name, GLboolean is_transpose,
                           const GLfloat* data) const;

 private:
  GLuint id_;

  GLuint CompileShader(GLenum type, const std::string& source) const;

  std::string ReadFile(const std::string& file_path) const;

  GLuint CreateShaderProgram(
      const std::string& vetex_shader_file_path,
      const std::string& fragment_shader_file_path) const;

  GLint GetUniformLocation(const std::string& name) const;
};

#endif
