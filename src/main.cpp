#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "camera.h"
#include "teapot.h"

#pragma region Constants

int width = 800;
int height = 600;

#pragma endregion

#pragma region GlobalVariables

// Camera.
Camera camera_;

// OpenGL IDs.
GLuint shader_program_id_;

// Keyboard presses.
std::vector<bool> keys_pressed(256, false);

#pragma endregion

GLuint CompileShader(GLenum type, const std::string& source) {
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

    std::cerr << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader: " << error_log << std::endl;

    glDeleteShader(shader);
    free(error_log);

    exit(1);
  }

  return shader;
}

std::string ReadFile(const std::string& file_path) {
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

GLuint CreateShaderProgram(const std::string& vetex_shader_file_path,
                           const std::string& fragment_shader_file_path) {
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

void GenerateObjectBufferTeapot() {
  GLuint loc1 = glGetAttribLocation(shader_program_id_, "vertex_position");
  GLuint loc2 = glGetAttribLocation(shader_program_id_, "vertex_normals");

  GLuint vp_vbo;
  glGenBuffers(1, &vp_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float),
               teapot_vertex_points, GL_STATIC_DRAW);
  GLuint vn_vbo;
  glGenBuffers(1, &vn_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float),
               teapot_normals, GL_STATIC_DRAW);

  GLuint teapot_vao;
  glGenVertexArrays(1, &teapot_vao);
  glBindVertexArray(teapot_vao);

  glEnableVertexAttribArray(loc1);
  glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
  glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(loc2);
  glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
  glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void InitialiseScene() {
  shader_program_id_ = CreateShaderProgram("shaders/simpleVertexShader.txt",
                                           "shaders/simpleFragmentShader.txt");

  camera_ = Camera::Camera();

  GenerateObjectBufferTeapot();
}

void Display() {
  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader_program_id_);

  // Declare your uniform variables that will be used in your shader
  int matrix_location = glGetUniformLocation(shader_program_id_, "model");
  int view_mat_location = glGetUniformLocation(shader_program_id_, "view");
  int proj_mat_location = glGetUniformLocation(shader_program_id_, "proj");

  // Here is where the code for the viewport lab will go, to get you started I
  // have drawn a t-pot in the bottom left The model transform rotates the
  // object by 45 degrees, the view transform sets the camera at -40 on the
  // z-axis, and the perspective projection is setup using Antons method

  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);
  glm::mat4 persp_proj(1.0f);

  model = glm::rotate<float>(glm::mat4(1.0f), glm::radians(45.0f),
                             glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale<float>(model, glm::vec3(0.1f, 0.1f, 0.1f));
  view = camera_.GetMatrix();
  persp_proj = glm::perspective<float>(45.0f, (float)width / (float)height,
                                       1.0f, 1000.0f);

  glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE,
                     glm::value_ptr(persp_proj));
  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

  glutSwapBuffers();
}

void UpdateScene() {
  // Wait until at least 16ms passed since start of last frame (Effectively caps
  // framerate at ~60fps)
  static DWORD last_time = 0;
  DWORD curr_time = timeGetTime();
  float delta = (curr_time - last_time) * 0.001f;
  if (delta > 0.03f) delta = 0.03f;
  last_time = curr_time;

  camera_.SetDelta(delta);
  camera_.UpdatePosition();

  // Draw the next frame
  glutPostRedisplay();
}

void OnMouseMove(int x, int y) { camera_.OnMouseMove(x, y); }

void OnKeyboardDown(unsigned char key, int x, int y) {
  if (key == 27) {
    exit(0);
  }

  std::cout << key << std::endl;

  camera_.OnKeyboardDown(key);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
  camera_.OnKeyboardUp(key);
}

int main(int argc, char** argv) {
  // Setup the window.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutCreateWindow("Climate Change Visualisation");
  glutSetCursor(GLUT_CURSOR_NONE);

  // Register callbacks.
  glutDisplayFunc(Display);
  glutIdleFunc(UpdateScene);
  glutKeyboardFunc(OnKeyboardDown);
  glutKeyboardUpFunc(OnKeyboardUp);
  glutPassiveMotionFunc(OnMouseMove);

  glewExperimental = GL_TRUE;
  GLenum result = glewInit();

  if (result != GLEW_OK) {
    std::cerr << "Error: " << glewGetErrorString(result) << std::endl;

    return 1;
  }

  // Setup objects and shaders.
  InitialiseScene();

  glutMainLoop();
  return 0;
}
