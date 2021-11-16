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
#include "mesh.h"
#include "polar_bear.h"

std::shared_ptr<Renderer> renderer_;
std::shared_ptr<Shader> shader_;
std::shared_ptr<Camera> camera_;
// std::shared_ptr<PolarBear> polar_bear_;
// std::vector<Mesh> meshes_;

void InitialiseScene() {
  renderer_->Init();

  renderer_ = std::shared_ptr<Renderer>(new Renderer());
  camera_ = std::shared_ptr<Camera>(new Camera());
  shader_ = std::shared_ptr<Shader>(new Shader(
      "shaders/simpleVertexShader.txt", "shaders/simpleFragmentShader.txt"));

  // meshes_.push_back(Mesh("models/floor.dae"));

  // polar_bear_ = std::shared_ptr<PolarBear>(new PolarBear());
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

  shader_->SetUniformMatrix4fv("view", GL_FALSE, glm::value_ptr(view));
  shader_->SetUniformMatrix4fv("proj", GL_FALSE, glm::value_ptr(persp_proj));

  // polar_bear_->UpdatePosition();
  camera_->UpdatePosition();

  // renderer_->Draw(meshes_, *shader_);
  // renderer_->Draw(polar_bear_->GetMeshes(), *shader_);

  glutSwapBuffers();
}

void UpdateScene() {
  // Wait until at least 16ms passed since start of last frame (Effectively caps
  // framerate at ~60fps)
  static float last_time = 0;
  float curr_time = (float)glutGet(GLUT_ELAPSED_TIME);
  float delta = curr_time - last_time;
  last_time = curr_time;

  // polar_bear_->SetDelta(delta);
  camera_->SetDelta(delta);

  // Draw the next frame
  glutPostRedisplay();
}

void OnMouseMove(int x, int y) { camera_->OnMouseMove(x, y); }

void OnKeyboardDown(unsigned char key, int x, int y) {
  if (key == 27) {
    glutLeaveMainLoop();
  }

  // polar_bear_->OnKeyboardDown(key);
  camera_->OnKeyboardDown(key);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
  // polar_bear_->OnKeyboardUp(key);
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
