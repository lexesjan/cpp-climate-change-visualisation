#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "player.h"

#include <iostream>

Player::Player(std::string path, Shader &shader, Renderer &renderer,
               float player_speed, Material material)
    : is_key_pressed_(256, false),
      model_matrix_(1.0f),
      player_speed_(player_speed),
      obstacles_(std::make_shared<std::vector<Obstacle>>()),
      AnimatedModel(path, shader, renderer, material) {}

void Player::UpdatePosition() {
  bool is_moving_forwards = false;
  bool is_moving_backwards = false;

  glm::vec3 previous_position = GetPosition();

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

  if (is_key_pressed_['g']) {
    Material shiny_material(glm::vec3(0.0f), 32.0f);

    SetMatrial(shiny_material);
  }

  if (is_key_pressed_['G']) {
    Material default_material(glm::vec3(0.0f));

    SetMatrial(default_material);
  }

  glm::vec3 current_position = GetPosition();

  float collision_offset = 2.0f;

  for (Obstacle &obstacle : *obstacles_) {
    float distance = glm::distance(
        glm::vec2(current_position.x, current_position.z), obstacle.position);

    if (distance > 0 && distance < (obstacle.radius + collision_offset)) {
      SetPosition(previous_position);
    }
  }

  if (is_key_pressed_['+']) {
    player_speed_ += 0.1f;
  }

  if (is_key_pressed_['-']) {
    player_speed_ -= 0.1f;
  }

  if (is_key_pressed_['r'] || is_key_pressed_['R']) {
    player_speed_ = 1.0f;
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

void Player::SetPosition(glm::vec3 &position) {
  model_matrix_[3] = glm::vec4(position, 1.0f);
}

void Player::SetDelta(float delta) { delta_ = delta * player_speed_; }

void Player::SetObstacles(std::shared_ptr<std::vector<Obstacle>> obstacles) {
  obstacles_ = obstacles;
}
