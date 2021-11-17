#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "vertex_buffer_object.h"
#include "vertex_array_object.h"
#include "camera.h"
#include "shader.h"
#include "renderer.h"
#include "model.h"

std::shared_ptr<Renderer> renderer_;
std::shared_ptr<Shader> shader_;
std::shared_ptr<Camera> camera_;
std::vector<Model> models_;

void InitialiseScene() {
  renderer_->Init();

  renderer_ = std::make_shared<Renderer>();
  camera_ = std::make_shared<Camera>();
  shader_ = std::make_shared<Shader>("shaders/simpleVertexShader.txt",
                                     "shaders/simpleFragmentShader.txt");

  models_.push_back(Model("models/polar_bear/body.fbx", *shader_, *renderer_));
}

void Display() {
  renderer_->Clear();

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);
  glm::mat4 persp_proj(1.0f);

  view = camera_->GetMatrix();
  persp_proj = glm::perspective<float>(45.0f, (float)width / (float)height,
                                       1.0f, 1000.0f);

  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

  shader_->Bind();
  shader_->SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view));
  shader_->SetUniformMatrix4fv("proj", GL_FALSE, glm::value_ptr(persp_proj));
  shader_->SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model));

  camera_->UpdatePosition();

  for (Model& model : models_) {
    model.Draw();
  }

  glutSwapBuffers();
}

void UpdateScene() {
  // Wait until at least 16ms passed since start of last frame (Effectively caps
  // framerate at ~60fps)
  static float last_time = 0;
  float curr_time = (float)glutGet(GLUT_ELAPSED_TIME);
  float delta = curr_time - last_time;
  last_time = curr_time;

  camera_->SetDelta(delta);

  // Draw the next frame
  glutPostRedisplay();
}

void OnMouseMove(int x, int y) { camera_->OnMouseMove(x, y); }

void OnKeyboardDown(unsigned char key, int x, int y) {
  if (key == 27) {
    glutLeaveMainLoop();
  }

  camera_->OnKeyboardDown(key);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
  camera_->OnKeyboardUp(key);
}

int main(int argc, char** argv) {
  // Setup the window.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800, 600);
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
