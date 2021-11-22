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
#include "animated_model.h"
#include "player.h"

std::unique_ptr<Renderer> renderer_;
std::unique_ptr<Shader> model_shader_;
std::unique_ptr<Shader> animated_model_shader_;
std::unique_ptr<Camera> camera_;
std::vector<AnimatedModel> models_;
std::unique_ptr<Player> player_;

void InitialiseScene() {
  renderer_->Init();

  renderer_ = std::make_unique<Renderer>();
  camera_ = std::make_unique<Camera>();
  model_shader_ = std::make_unique<Shader>("shaders/model_shader.vs",
                                           "shaders/model_shader.fs");

  animated_model_shader_ = std::make_unique<Shader>(
      "shaders/animated_model_shader.vs", "shaders/model_shader.fs");

  // models_.push_back(AnimatedModel("models/rabbit/body.fbx",
  //                                *animated_model_shader_, *renderer_));
  // models_.push_back(AnimatedModel("models/polar_bear/body.fbx",
  //                                *animated_model_shader_, *renderer_));

  player_ = std::make_unique<Player>(*animated_model_shader_, *renderer_);
}

void Display() {
  renderer_->Clear();

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  glm::mat4 model_mat(1.0f);
  glm::mat4 view_mat(1.0f);
  glm::mat4 persp_proj_mat(1.0f);

  view_mat = camera_->GetMatrix();
  persp_proj_mat = glm::perspective<float>(45.0f, (float)width / (float)height,
                                           1.0f, 1000.0f);

  model_shader_->Bind();
  model_shader_->SetUniformMatrix4fv("view", GL_FALSE,
                                     glm::value_ptr(view_mat));
  model_shader_->SetUniformMatrix4fv("proj", GL_FALSE,
                                     glm::value_ptr(persp_proj_mat));
  model_shader_->SetUniformMatrix4fv("model", GL_FALSE,
                                     glm::value_ptr(model_mat));

  camera_->UpdatePosition();

  animated_model_shader_->Bind();
  animated_model_shader_->SetUniformMatrix4fv("view", GL_FALSE,
                                              glm::value_ptr(view_mat));
  animated_model_shader_->SetUniformMatrix4fv("proj", GL_FALSE,
                                              glm::value_ptr(persp_proj_mat));
  animated_model_shader_->SetUniformMatrix4fv("model", GL_FALSE,
                                              glm::value_ptr(model_mat));

  for (unsigned int i = 0; i < models_.size(); i++) {
    AnimatedModel& model = models_[i];

    model.UpdateBoneTransformations();
    const std::vector<glm::mat4>& transforms = model.GetFinalTransforms();

    animated_model_shader_->SetUniformMatrix4fv(
        "final_bone_transforms", GL_FALSE, glm::value_ptr(transforms.front()),
        (GLsizei)transforms.size());

    model_mat =
        glm::translate(glm::mat4(1.0f), glm::vec3(5.0f * i, 0.0f, 0.0f));

    animated_model_shader_->SetUniformMatrix4fv("model", GL_FALSE,
                                                glm::value_ptr(model_mat));

    model.Draw();
  }

  player_->UpdatePosition();
  player_->Draw();

  glutSwapBuffers();
}

void UpdateScene() {
  // Wait until at least 16ms passed since start of last frame (Effectively caps
  // framerate at ~60fps)
  static float last_time = 0;
  float curr_time = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000;
  float delta = curr_time - last_time;
  last_time = curr_time;

  camera_->SetDelta(delta);

  for (AnimatedModel& model : models_) {
    model.SetDelta(delta);
  }

  player_->SetDelta(delta);

  // Draw the next frame
  glutPostRedisplay();
}

void OnMouseMove(int x, int y) { camera_->OnMouseMove(x, y); }

void OnKeyboardDown(unsigned char key, int x, int y) {
  if (key == 27) {
    glutLeaveMainLoop();
  }

  camera_->OnKeyboardDown(key);
  player_->OnKeyboardDown(key);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
  camera_->OnKeyboardUp(key);
  player_->OnKeyboardUp(key);
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
