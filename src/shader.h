#ifndef CLIMATE_CHANGE_VISUALISATION_SHADER_H_
#define CLIMATE_CHANGE_VISUALISATION_SHADER_H_

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Shader {
 public:
  explicit Shader();
  explicit Shader(const std::string& vetex_shader_file_path,
                  const std::string& fragment_shader_file_path);

  void Bind() const;

  void Unbind() const;

  void SetUniform1i(const std::string& name, const GLint data) const;

  void SetUniform1f(const std::string& name, const GLfloat data) const;

  void SetUniformMatrix4fv(const std::string& name, GLboolean is_transpose,
                           const GLfloat* data, GLsizei count = 1) const;

 private:
  GLuint id_;
  std::unordered_map<std::string, GLint> uniform_location_cache_;

  GLuint CompileShader(GLenum type, const std::string& source) const;

  std::string ReadFile(const std::string& file_path) const;

  GLuint CreateShaderProgram(
      const std::string& vetex_shader_file_path,
      const std::string& fragment_shader_file_path) const;

  GLint GetUniformLocation(const std::string& name) const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_SHADER_H_
