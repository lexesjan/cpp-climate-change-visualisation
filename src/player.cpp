#include <glm/gtc/type_ptr.hpp>
#include "player.h"

#include <iostream>

Player::Player(Shader shader, Renderer renderer)
    : is_key_pressed_(256, false),
      model_matrix_(1.0f),
      AnimatedModel("models/polar_bear/body.fbx", shader, renderer) {}

void Player::UpdatePosition() {
  bool is_moving_forwards = false;
  bool is_moving_backwards = false;
  if (is_key_pressed_['i']) {
    model_matrix_ =
        glm::translate(model_matrix_, delta_ * glm::vec3(0.0f, 0.0f, 0.5f));
    is_moving_forwards = true;
  }

  if (is_key_pressed_['k']) {
    model_matrix_ =
        glm::translate(model_matrix_, delta_ * glm::vec3(0.0f, 0.0f, -0.5f));
    is_moving_backwards = true;
  }

  if (is_key_pressed_['j']) {
    model_matrix_ = glm::rotate(model_matrix_, glm::radians(25.0f) * delta_,
                                glm::vec3(0.0f, 1.0f, 0.0f));
    is_moving_forwards = true;
  }

  if (is_key_pressed_['l']) {
    model_matrix_ = glm::rotate(model_matrix_, glm::radians(25.0f) * delta_,
                                glm::vec3(0.0f, -1.0f, 0.0f));
    is_moving_forwards = true;
  }

  if (is_moving_forwards || is_moving_backwards) {
    UpdateBoneTransformations(is_moving_backwards);
  }
}

void Player::OnKeyboardDown(unsigned char key) { is_key_pressed_[key] = true; }

void Player::OnKeyboardUp(unsigned char key) { is_key_pressed_[key] = false; }

void Player::Draw() {
  const std::vector<glm::mat4>& transforms = GetFinalTransforms();

  shader_.SetUniformMatrix4fv("final_bone_transforms", GL_FALSE,
                              glm::value_ptr(transforms.front()),
                              (GLsizei)transforms.size());

  shader_.SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model_matrix_));
  AnimatedModel::Draw();
}
