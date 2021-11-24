#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "application.h"
#include "point_light.h"
#include "directed_light.h"
#include "material.h"

Application::Application()
    : renderer_(),
      camera_(),
      animated_model_shader_("shaders/animated_model_shader.vs",
                             "shaders/lighting_shader.fs"),
      model_shader_("shaders/model_shader.vs", "shaders/lighting_shader.fs"),
      light_source_shader_("shaders/model_shader.vs",
                           "shaders/light_source_shader.fs"),
      player_(animated_model_shader_, renderer_),
      monkey_head_(
          "assets/monkey_head/body.fbx", model_shader_, renderer_,
          Material(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f),
                   glm::vec3(1.0f), 32.0f)),
      directed_light_(glm::vec3(0.2f), glm::vec3(0.0f, -1.0f, 0.0f)),
      campfire_("assets/campfire/body.fbx", glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(10.0f, 0.0f, 0.0f), light_source_shader_, renderer_),
      cube_("assets/cube/body.fbx", glm::vec3(1.0f),
            glm::vec3(-10.0f, 0.0f, 0.0f), light_source_shader_, renderer_,
            Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f))),
      last_time_(0.0f) {
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

  model_shader_.Bind();
  model_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                    glm::value_ptr(model_mat));
  model_shader_.SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view_mat));
  model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                    glm::value_ptr(persp_proj_mat));

  directed_light_.Set("directed_light", model_shader_);
  cube_.Set("point_light", model_shader_, 0);
  campfire_.Set("point_light", model_shader_, 1);

  model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  monkey_head_.Draw();

  animated_model_shader_.Bind();
  animated_model_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                             glm::value_ptr(view_mat));
  animated_model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                             glm::value_ptr(persp_proj_mat));

  directed_light_.Set("directed_light", animated_model_shader_);
  cube_.Set("point_light", animated_model_shader_, 0);
  campfire_.Set("point_light", animated_model_shader_, 1);

  animated_model_shader_.SetUniform1f("material.shininess", 1.0f);
  animated_model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  player_.Draw();

  light_source_shader_.Bind();
  light_source_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                           glm::value_ptr(view_mat));
  light_source_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                           glm::value_ptr(persp_proj_mat));

  campfire_.Draw();
  cube_.Draw();

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
