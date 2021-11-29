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

#include <stdio.h>

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
      rock_positions_({{glm::vec3(-5.94219f, 0.0f, 17.0918f), 0.0f},
                       {glm::vec3(-16.4027f, 0.0f, 0.0f), -43.6154f},
                       {glm::vec3(-12.7405f, 0.0f, -19.2361f), -75.0218f}}),
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

  campfires_.push_back(LightSource(
      "assets/campfire/body.fbx", glm::vec3(0.949f, 0.490f, 0.047f),
      glm::vec3(0.041548f, 0.0f, -5.25218f), basic_shader_, renderer_));
  campfires_.push_back(LightSource(
      "assets/campfire/body.fbx", glm::vec3(0.501f, 0.035f, 0.035f),
      glm::vec3(-5.63723f, 0.0f, 0.11825f), basic_shader_, renderer_));
  campfires_.push_back(LightSource(
      "assets/campfire/body.fbx", glm::vec3(0.862f, 0.960f, 0.031f),
      glm::vec3(3.51629f, 0.0f, 2.96193f), basic_shader_, renderer_));

  for (unsigned int i = 0; i < campfires_.size(); i++) {
    campfires_[i].Set("point_light", animated_model_shader_, i);
    campfires_[i].Set("point_light", model_shader_, i);
  }

  AnimatedModel rabbit = AnimatedModel("assets/rabbit/body.fbx",
                                       animated_model_shader_, renderer_);

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  Material emerald(glm::vec3(0.0215f, 0.1745f, 0.0215f),
                   glm::vec3(0.07568f, 0.61424f, 0.07568f),
                   glm::vec3(0.633f, 0.727811f, 0.633f), 76.8f);
  Material bronze(glm::vec3(0.2125f, 0.1275f, 0.054f),
                  glm::vec3(0.714f, 0.4284f, 0.18144f),
                  glm::vec3(0.393548f, 0.271906f, 0.166721f), 25.6f);
  Material gold(glm::vec3(0.24725f, 0.1995f, 0.0745f),
                glm::vec3(0.75164f, 0.60648f, 0.22648f),
                glm::vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
  Material black_plastic(glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.01f, 0.01f, 0.01f),
                         glm::vec3(0.50f, 0.50f, 0.50f), 32.0f);
  Material chrome(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f),
                  glm::vec3(0.774597f, 0.774597f, 0.774597f), 76.8f);
  Material default_material(glm::vec3(0.0f), 1.0f);

  for (unsigned int i = 0; i < 10; i++) {
    switch (i) {
      case 0:
        rabbit.SetMatrial(emerald);
        rabbit.SetUseTexture(false);
        break;
      case 1:
        rabbit.SetMatrial(gold);
        rabbit.SetUseTexture(false);
        break;
      case 2:
        rabbit.SetMatrial(black_plastic);
        rabbit.SetUseTexture(false);
        break;
      case 3:
        rabbit.SetMatrial(chrome);
        rabbit.SetUseTexture(false);
        break;
      case 4:
        rabbit.SetMatrial(bronze);
        rabbit.SetUseTexture(false);
        break;
      default:
        rabbit.SetMatrial(default_material);
        rabbit.SetUseTexture(true);
        break;
    }

    boids_.push_back(Boid(
        glm::vec2(random_utils::range(-10, 10), random_utils::range(-10, 10)),
        rabbit, 30.0f));
  }
}

void Application::Display() {
  renderer_.Clear();

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

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
                                             glm::value_ptr(glm::mat4(1.0f)));
  animated_model_shader_.SetUniformMatrix4fv("view", GL_FALSE,
                                             glm::value_ptr(view_mat));
  animated_model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                             glm::value_ptr(persp_proj_mat));
  animated_model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  player_.Draw();

  for (Boid &boid : boids_) {
    boid.Draw();
  }

  basic_shader_.Bind();
  basic_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                    glm::value_ptr(glm::mat4(1.0f)));
  basic_shader_.SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view_mat));
  basic_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                    glm::value_ptr(persp_proj_mat));

  for (unsigned int i = 0; i < campfires_.size(); i++) {
    campfires_[i].Draw();
  }

  model_shader_.Bind();
  model_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                    glm::value_ptr(glm::mat4(1.0f)));
  model_shader_.SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view_mat));
  model_shader_.SetUniformMatrix4fv("proj", GL_FALSE,
                                    glm::value_ptr(persp_proj_mat));
  model_shader_.SetUniform3f("view_position", camera_.GetPosition());

  platform_.Draw();

  for (ModelPosition &rock_position : rock_positions_) {
    glm::mat4 model_mat(1.0f);
    model_mat = glm::translate(model_mat, rock_position.position);
    model_mat = glm::rotate(model_mat, glm::radians(rock_position.rotation),
                            glm::vec3(0.0f, 1.0f, 0.0f));

    model_shader_.SetUniformMatrix4fv("model", GL_FALSE,
                                      glm::value_ptr(model_mat));
    rock_.Draw();
  }

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

  std::shared_ptr<std::vector<Obstacle>> obstacles =
      std::make_shared<std::vector<Obstacle>>(std::vector<Obstacle>{
          {glm::vec2(player_position.x, player_position.z), 1.87f}});

  for (ModelPosition &rock_position : rock_positions_) {
    obstacles->push_back(
        {glm::vec2(rock_position.position.x, rock_position.position.z),
         4.135f});
  }

  for (Boid &boid : boids_) {
    boid.SetDelta(delta);
    boid.SetObstacles(obstacles);
    boid.UpdatePosition(boids_);
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
