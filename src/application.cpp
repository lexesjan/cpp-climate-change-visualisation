#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "application.h"

Application::Application()
    : camera_(),
      renderer_(),
      last_time_(0),
      lighting_shader_("shaders/model_shader.vs", "shaders/model_shader.fs"),
      animated_model_shader_("shaders/animated_model_shader.vs",
                             "shaders/model_shader.fs"),
      player_(animated_model_shader_, renderer_),
      monkey_head_("models/monkey_head/body.fbx", lighting_shader_, renderer_) {
  renderer_.Init();
}

void Application::Display() {
  renderer_.Clear();

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  glm::mat4 model_mat(1.0f);
  glm::mat4 view_mat(1.0f);
  glm::mat4 persp_proj_mat(1.0f);

  view_mat = camera_.GetMatrix();
  persp_proj_mat = glm::perspective<float>(45.0f, (float)width / (float)height,
                                           1.0f, 1000.0f);

  model_mat =
      glm::rotate(model_mat, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

  lighting_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                       glm::value_ptr(model_mat));
  lighting_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                       glm::value_ptr(view_mat));
  lighting_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                       glm::value_ptr(persp_proj_mat));

  monkey_head_.Draw();

  // animated_model_shader_.SetUniformMatrix4fv("view", GL_FALSE,
  //                                           glm::value_ptr(view_mat));
  // animated_model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
  //                                           glm::value_ptr(persp_proj_mat));

  // player_.Draw();

  glutSwapBuffers();
}

void Application::UpdateScene() {
  float curr_time = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000;
  float delta = curr_time - last_time_;
  last_time_ = curr_time;

  camera_.SetDelta(delta);
  camera_.UpdatePosition();

  // player_.SetDelta(delta);
  // player_.UpdatePosition();

  glutPostRedisplay();
}

void Application::OnMouseMove(int x, int y) { camera_.OnMouseMove(x, y); }

void Application::OnKeyboardDown(unsigned char key, int x, int y) {
  if (key == 27) {
    glutLeaveMainLoop();
  }

  camera_.OnKeyboardDown(key);
  player_.OnKeyboardDown(key);
}

void Application::OnKeyboardUp(unsigned char key, int x, int y) {
  camera_.OnKeyboardUp(key);
  player_.OnKeyboardUp(key);
}
