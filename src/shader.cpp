#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.h"

Shader::Shader() : id_(0) {}

Shader::Shader(const std::string& vertex_shader_file_path,
               const std::string& fragment_shader_file_path) {
  id_ = CreateShaderProgram(vertex_shader_file_path, fragment_shader_file_path);
}

void Shader::Bind() const { glUseProgram(id_); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetUniform1i(const std::string& name, const GLint data) {
  Bind();
  glUniform1i(GetUniformLocation(name), data);
}

void Shader::SetUniform1f(const std::string& name, const GLfloat data) {
  Bind();
  glUniform1f(GetUniformLocation(name), data);
}

void Shader::SetUniform3f(const std::string& name, const GLfloat data1,
                          const GLfloat data2, const GLfloat data3) {
  Bind();
  glUniform3f(GetUniformLocation(name), data1, data2, data3);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 data) {
  Bind();
  glUniform3f(GetUniformLocation(name), data.x, data.y, data.z);
}

void Shader::SetUniformMatrix4fv(const std::string& name,
                                 GLboolean is_transpose, const GLfloat* data,
                                 GLsizei count) {
  Bind();
  glUniformMatrix4fv(GetUniformLocation(name), count, is_transpose, data);
}

GLuint Shader::CompileShader(GLenum type, const std::string& source) const {
  GLuint shader = glCreateShader(type);

  const char* src = source.c_str();

  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint is_success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &is_success);

  if (is_success == GL_FALSE) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    char* error_log = (char*)malloc(sizeof(char) * length);
    glGetShaderInfoLog(shader, length, &length, error_log);

    std::cerr << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader: " << error_log << std::endl;

    glDeleteShader(shader);
    free(error_log);

    exit(1);
  }

  return shader;
}

std::string Shader::ReadFile(const std::string& file_path) const {
  std::ifstream file(file_path);
  if (file.fail()) {
    std::cerr << "Failed reading file " << file_path << std::endl;
    exit(1);
  }

  std::stringstream stream;
  stream << file.rdbuf();
  file.close();

  return stream.str();
}

GLuint Shader::CreateShaderProgram(
    const std::string& vetex_shader_file_path,
    const std::string& fragment_shader_file_path) const {
  std::string vertex_shader_source = ReadFile(vetex_shader_file_path);
  std::string fragment_shader_source = ReadFile(fragment_shader_file_path);

  GLuint program = glCreateProgram();
  GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
  GLuint fragment_shader =
      CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  // Link shaders to program.
  glLinkProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  GLint is_success;
  glGetProgramiv(program, GL_LINK_STATUS, &is_success);

  if (is_success == GL_FALSE) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    char* error_log = (char*)malloc(sizeof(char) * length);
    glGetProgramInfoLog(program, length, nullptr, error_log);

    std::cerr << "Failed to link program: " << error_log << std::endl;

    glDeleteProgram(program);
    free(error_log);

    exit(1);
  }

  // Validate program.
  glValidateProgram(program);

  glGetProgramiv(program, GL_VALIDATE_STATUS, &is_success);

  if (is_success == GL_FALSE) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    char* error_log = (char*)malloc(sizeof(char) * length);
    glGetProgramInfoLog(program, length, nullptr, error_log);

    std::cerr << "Failed to validate program: " << error_log << std::endl;

    glDeleteProgram(program);
    free(error_log);

    exit(1);
  }

  return program;
}

GLint Shader::GetUniformLocation(const std::string& name) {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
    return uniform_location_cache_[name];
  }

  uniform_location_cache_[name] = glGetUniformLocation(id_, name.c_str());

  return uniform_location_cache_[name];
}
