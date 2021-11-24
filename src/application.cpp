#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "application.h"
#include "point_light.h"
#include "directed_light.h"

Application::Application()
    : camera_(),
      renderer_(),
      last_time_(0),
      lighting_shader_("shaders/lighting_shader.vs",
                       "shaders/lighting_shader.fs"),
      animated_model_shader_("shaders/animated_model_shader.vs",
                             "shaders/lighting_shader.fs"),
      default_texture_("models/white.png"),
      player_(animated_model_shader_, renderer_),
      monkey_head_("models/monkey_head/body.fbx", lighting_shader_, renderer_,
                   &default_texture_) {
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

  model_mat = glm::translate(model_mat, glm::vec3(0.0f, -10.0f, 0.0f));
  model_mat = glm::scale(model_mat, glm::vec3(10.0f));

  lighting_shader_.Bind();
  lighting_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                       glm::value_ptr(model_mat));
  lighting_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                       glm::value_ptr(view_mat));
  lighting_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                       glm::value_ptr(persp_proj_mat));

  DirectedLight directed_light(glm::vec3(0.0f, 0.0f, 1.0f) * 0.1f,
                               glm::vec3(0.0f, -1.0f, 0.0f));
  PointLight point_light_white(glm::vec3(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
  PointLight point_light_yellow(glm::vec3(1.0f, 1.0f, 0.0f),
                                glm::vec3(10.0f, 0.0f, 0.0f));

  directed_light.Set("directed_light", lighting_shader_);
  point_light_white.Set("point_light", lighting_shader_, 0);
  point_light_yellow.Set("point_light", lighting_shader_, 1);

  lighting_shader_.SetUniform1f("material.shininess", 1.0f);
  lighting_shader_.SetUniform3f("view_position", camera_.GetPosition());

  monkey_head_.Draw();

  animated_model_shader_.Bind();
  animated_model_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                             glm::value_ptr(view_mat));
  animated_model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                             glm::value_ptr(persp_proj_mat));

  directed_light.Set("directed_light", animated_model_shader_);
  point_light_white.Set("point_light", animated_model_shader_, 0);
  point_light_yellow.Set("point_light", animated_model_shader_, 1);

  animated_model_shader_.SetUniform1f("material.shininess", 1.0f);
  animated_model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  player_.Draw();

  glutSwapBuffers();
}

void Application::UpdateScene() {
  float curr_time = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000;
  float delta = curr_time - last_time_;
  last_time_ = curr_time;

  camera_.SetDelta(delta);
  camera_.UpdatePosition();

  player_.SetDelta(delta);
  player_.UpdatePosition();

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
