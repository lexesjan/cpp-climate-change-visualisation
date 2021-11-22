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

std::unique_ptr<Renderer> renderer_;
std::unique_ptr<Shader> model_shader_;
std::unique_ptr<Shader> animated_model_shader_;
std::unique_ptr<Camera> camera_;
std::unique_ptr<Model> polar_bear_;
std::vector<Model> models_;

void InitialiseScene() {
  renderer_->Init();

  renderer_ = std::make_unique<Renderer>();
  camera_ = std::make_unique<Camera>();
  model_shader_ = std::make_unique<Shader>("shaders/model_shader.vs",
                                           "shaders/model_shader.fs");

  animated_model_shader_ = std::make_unique<Shader>(
      "shaders/animated_model_shader.vs", "shaders/model_shader.fs");
  polar_bear_ = std::make_unique<Model>("models/polar_bear/body.fbx",
                                        *animated_model_shader_, *renderer_);
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

  model_shader_->Bind();
  model_shader_->SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view));
  model_shader_->SetUniformMatrix4fv("proj", GL_FALSE,
                                     glm::value_ptr(persp_proj));
  model_shader_->SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model));

  camera_->UpdatePosition();

  animated_model_shader_->Bind();
  animated_model_shader_->SetUniformMatrix4fv("view", GL_FALSE,
                                              glm::value_ptr(view));
  animated_model_shader_->SetUniformMatrix4fv("proj", GL_FALSE,
                                              glm::value_ptr(persp_proj));
  animated_model_shader_->SetUniformMatrix4fv("model", GL_FALSE,
                                              glm::value_ptr(model));

  polar_bear_->UpdateBoneTransformations();
  const std::vector<glm::mat4>& transforms = polar_bear_->GetFinalTransforms();

  animated_model_shader_->SetUniformMatrix4fv(
      "final_bone_transforms", GL_FALSE, glm::value_ptr(transforms.front()),
      (GLsizei)transforms.size());

  polar_bear_->Draw();

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
  polar_bear_->SetDelta(delta);

  for (Model& model : models_) {
    model.SetDelta(delta);
  }

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
