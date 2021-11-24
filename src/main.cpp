#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>
#include <iostream>
#include "application.h"

std::unique_ptr<Application> application_;

void Display() { application_->Display(); }

void UpdateScene() { application_->UpdateScene(); }

void OnKeyboardDown(unsigned char key, int x, int y) {
  application_->OnKeyboardDown(key, x, y);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
  application_->OnKeyboardUp(key, x, y);
}

void OnMouseMove(int x, int y) { application_->OnMouseMove(x, y); }

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
  application_ = std::make_unique<Application>();

  glutMainLoop();

  return 0;
}
