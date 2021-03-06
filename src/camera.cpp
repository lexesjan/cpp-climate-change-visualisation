#include <GL/freeglut.h>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "camera.h"

Camera::Camera()
    : last_x_(0),
      last_y_(0),
      is_first_mouse_movement_(true),
      yaw_(-230.0f),
      pitch_(-30.0f),
      delta_(0.0f),
      is_key_pressed_(256, false),
      camera_pos_(10.0f, 10.0f, -10.0f),
      camera_front_(0.0f, 0.0f, -1.0f),
      camera_up_(0.0f, 1.0f, 0.0f) {}

void Camera::OnMouseMove(int x, int y) {
  if (is_first_mouse_movement_) {
    last_x_ = x;
    last_y_ = y;
    is_first_mouse_movement_ = false;
  }

  float x_offset = (float)(x - last_x_);
  float y_offset = (float)(last_y_ - y);

  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  glutWarpPointer(width / 2, height / 2);

  last_x_ = width / 2;
  last_y_ = height / 2;

  float sensitivity = 0.1f;
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  yaw_ += x_offset;
  pitch_ += y_offset;

  if (pitch_ > 89.0f) {
    pitch_ = 89.0f;
  }

  if (pitch_ < -89.0f) {
    pitch_ = -89.0f;
  }

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  direction.y = sin(glm::radians(pitch_));
  direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  camera_front_ = glm::normalize(direction);
}

void Camera::OnKeyboardDown(unsigned char key) { is_key_pressed_[key] = true; }

void Camera::OnKeyboardUp(unsigned char key) { is_key_pressed_[key] = false; }

const glm::vec3& Camera::GetPosition() const { return camera_pos_; }

void Camera::UpdatePosition() {
  float camera_speed = 8.00f * delta_;

  if (is_key_pressed_['w']) {
    camera_pos_ += camera_speed * camera_front_;
  }

  if (is_key_pressed_['s']) {
    camera_pos_ -= camera_speed * camera_front_;
  }

  if (is_key_pressed_['a']) {
    camera_pos_ -=
        glm::normalize(glm::cross(camera_front_, camera_up_)) * camera_speed;
  }

  if (is_key_pressed_['d']) {
    camera_pos_ +=
        glm::normalize(glm::cross(camera_front_, camera_up_)) * camera_speed;
  }
}

void Camera::SetDelta(float delta) { delta_ = delta; }

glm::mat4 Camera::GetMatrix() {
  return glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);
}
