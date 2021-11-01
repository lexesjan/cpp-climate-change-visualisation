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
#include "vertex_buffer_object.h"
#include "vertex_array_object.h"
#include "camera.h"
#include "teapot.h"
#include "shader.h"
#include "renderer.h"

int width_ = 800;
int height_ = 600;
Renderer renderer_;
Shader shader_;
Camera camera_;
VertexArrayObject* teapot_vao_;

void GenerateObjectBufferTeapot() {
  std::vector<glm::vec3> teapot_vertices;

  for (int i = 0; i < teapot_vertex_count * 3; i += 3) {
    glm::vec3 location(teapot_vertex_points[i], teapot_vertex_points[i + 1],
                       teapot_vertex_points[i + 2]);
    teapot_vertices.push_back(location);

    glm::vec3 normal(teapot_normals[i], teapot_normals[i + 1],
                     teapot_normals[i + 2]);
    teapot_vertices.push_back(normal);
  }

  VertexBufferObject vbo(
      teapot_vertices.data(),
      teapot_vertices.size() * sizeof(teapot_vertices.front()));

  VertexBufferLayout layout;
  layout.AddElement<float>(3);
  layout.AddElement<float>(3);

  teapot_vao_ = new VertexArrayObject();
  teapot_vao_->AddBuffer(vbo, layout);
}

void InitialiseScene() {
  renderer_.Init();

  shader_ = Shader("shaders/simpleVertexShader.txt",
                   "shaders/simpleFragmentShader.txt");

  GenerateObjectBufferTeapot();
}

void Display() {
  renderer_.Clear();

  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);
  glm::mat4 persp_proj(1.0f);

  model = glm::rotate<float>(glm::mat4(1.0f), glm::radians(45.0f),
                             glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale<float>(model, glm::vec3(0.1f, 0.1f, 0.1f));
  view = camera_.GetMatrix();
  persp_proj = glm::perspective<float>(45.0f, (float)width_ / (float)height_,
                                       1.0f, 1000.0f);

  shader_.SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model));
  shader_.SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view));
  shader_.SetUniformMatrix4fv("proj", GL_FALSE, glm::value_ptr(persp_proj));

  renderer_.Draw(*teapot_vao_, shader_, teapot_vertex_count);

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

  camera_.OnKeyboardDown(key);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
  camera_.OnKeyboardUp(key);
}

int main(int argc, char** argv) {
  // Setup the window.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width_, height_);
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
