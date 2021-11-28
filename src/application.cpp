#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "application.h"
#include "point_light.h"
#include "directed_light.h"
#include "material.h"
#include "random_utils.h"

Application::Application()
    : renderer_(),
      camera_(),
      animated_model_shader_("shaders/animated_model_shader.vs",
                             "shaders/lighting_shader.fs"),
      model_shader_("shaders/model_shader.vs", "shaders/lighting_shader.fs"),
      basic_shader_("shaders/model_shader.vs", "shaders/basic_shader.fs"),
      skybox_shader_("shaders/skybox_shader.vs", "shaders/skybox_shader.fs"),
      directed_light_(glm::vec3(0.3f), glm::vec3(0.0f, -1.0f, 0.0f)),
      player_("assets/polar_bear/body.fbx", animated_model_shader_, renderer_,
              10.0f),
      platform_("assets/platform/body.fbx", model_shader_, renderer_),
      rock_("assets/rock/body.fbx", model_shader_, renderer_),
      skybox_({"assets/skybox/right.jpg", "assets/skybox/left.jpg",
               "assets/skybox/top.jpg", "assets/skybox/bottom.jpg",
               "assets/skybox/front.jpg", "assets/skybox/back.jpg"},
              skybox_shader_, renderer_),
      last_time_(0.0f) {
  Init();
}

void Application::Init() {
  std::srand((unsigned int)std::time(0));

  renderer_.Init();
  directed_light_.Set("directed_light", animated_model_shader_);
  directed_light_.Set("directed_light", model_shader_);

  campfires_.push_back(
      LightSource("assets/campfire/body.fbx", glm::vec3(0.949f, 0.490f, 0.047f),
                  glm::vec3(3.0f, 0.0f, 6.0f), basic_shader_, renderer_));
  campfires_.push_back(
      LightSource("assets/campfire/body.fbx", glm::vec3(0.501f, 0.035f, 0.035f),
                  glm::vec3(-6.0f, 0.0f, 3.0f), basic_shader_, renderer_));
  campfires_.push_back(
      LightSource("assets/campfire/body.fbx", glm::vec3(0.862f, 0.960f, 0.031f),
                  glm::vec3(-5.0f, 0.0f, -3.0f), basic_shader_, renderer_));

  for (unsigned int i = 0; i < campfires_.size(); i++) {
    campfires_[i].Set("point_light", animated_model_shader_, i);
    campfires_[i].Set("point_light", model_shader_, i);
  }

  AnimatedModel rabbit = AnimatedModel("assets/rabbit/body.fbx",
                                       animated_model_shader_, renderer_);

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  for (unsigned int i = 0; i < 10; i++) {
    boids_.push_back(Boid(
        glm::vec2(random_utils::range(-10, 10), random_utils::range(-10, 10)),
        rabbit));
  }
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
                                           0.1f, 1000.0f);

  skybox_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                     glm::value_ptr(persp_proj_mat));
  skybox_shader_.SetUniformMatrix4fv(
      "view", GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view_mat))));
  skybox_.Draw();

  animated_model_shader_.Bind();
  animated_model_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                             glm::value_ptr(model_mat));
  animated_model_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                             glm::value_ptr(view_mat));
  animated_model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                             glm::value_ptr(persp_proj_mat));
  animated_model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  player_.Draw();

  for (Boid boid : boids_) {
    boid.Draw();
  }

  basic_shader_.Bind();
  basic_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                    glm::value_ptr(model_mat));
  basic_shader_.SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view_mat));
  basic_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                    glm::value_ptr(persp_proj_mat));

  for (unsigned int i = 0; i < campfires_.size(); i++) {
    campfires_[i].Draw();
  }

  model_shader_.Bind();
  model_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                    glm::value_ptr(model_mat));
  model_shader_.SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view_mat));
  model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                    glm::value_ptr(persp_proj_mat));
  model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  platform_.Draw();

  model_shader_.SetUniformMatrix4fv(
      "model", GL_FALSE,
      glm::value_ptr(
          glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 0.0f))));

  rock_.Draw();

  model_shader_.SetUniformMatrix4fv(
      "model", GL_FALSE,
      glm::value_ptr(
          glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, 0.0f))));

  rock_.Draw();

  model_shader_.SetUniformMatrix4fv(
      "model", GL_FALSE,
      glm::value_ptr(
          glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, 0.0f, 13.0f))));

  rock_.Draw();

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

  glm::vec3 player_position = player_.GetPosition();

  std::vector<glm::vec2> obstacles{
      glm::vec2(player_position.x, player_position.z)};

  for (Boid &boid : boids_) {
    boid.SetDelta(delta);
    boid.UpdatePosition(boids_, obstacles, 30.0f);
  }

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
