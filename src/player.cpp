#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "player.h"

Player::Player(std::string path, Shader &shader, Renderer &renderer,
               float player_speed, Material material)
    : is_key_pressed_(256, false),
      model_matrix_(1.0f),
      player_speed_(player_speed),
      AnimatedModel(path, shader, renderer, material) {}

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
    model_matrix_ = glm::rotate(model_matrix_, delta_ * glm::radians(25.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    is_moving_forwards = true;
  }

  if (is_key_pressed_['l']) {
    model_matrix_ = glm::rotate(model_matrix_, delta_ * glm::radians(25.0f),
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
  shader_.SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model_matrix_));

  AnimatedModel::Draw();
}

const glm::vec3 Player::GetPosition() const {
  return glm::vec3(model_matrix_[3]);
}

void Player::SetDelta(float delta) { delta_ = delta * player_speed_; }
