#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "polar_bear.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

PolarBear::PolarBear() {
  delta_ = 0.0f;
  rotation_ = 0.0f;
  d_rotation_ = 1.0f;
  is_moving_ = false;
  is_key_pressed_ = std::vector<bool>(256, false);

  model_ = glm::mat4(1.0f);

  meshes_.push_back(Mesh("models/polar_bear_body.dae"));
  meshes_.push_back(Mesh("models/polar_bear_front_leg.dae"));
  meshes_.push_back(Mesh("models/polar_bear_front_leg.dae"));
  meshes_.push_back(Mesh("models/polar_bear_back_leg.dae"));
  meshes_.push_back(Mesh("models/polar_bear_back_leg.dae"));
}

const std::vector<Mesh>& PolarBear::GetMeshes() const { return meshes_; }

void PolarBear::SetDelta(float delta) { delta_ = delta; }

void PolarBear::UpdatePosition() {
  if (is_moving_) {
    rotation_ += d_rotation_;
  } else if (rotation_ != 0.0f) {
    rotation_ += d_rotation_;
  }

  if (rotation_ >= 45.0f) {
    d_rotation_ = -1.0f;
  }

  if (rotation_ <= -45.0f) {
    d_rotation_ = 1.0f;
  }

  if (is_key_pressed_['i']) {
    model_ = glm::translate(model_, delta_ * glm::vec3(0.0f, 0.0f, -5.0f));
    is_moving_ = true;
  }

  if (is_key_pressed_['k']) {
    model_ = glm::translate(model_, delta_ * glm::vec3(0.0f, 0.0f, 5.0f));
    is_moving_ = true;
  }

  if (is_key_pressed_['j']) {
    model_ = glm::rotate(model_, glm::radians(180.0f) * delta_,
                         glm::vec3(0.0f, 1.0f, 0.0f));
    is_moving_ = true;
  }

  if (is_key_pressed_['l']) {
    model_ = glm::rotate(model_, glm::radians(180.0f) * delta_,
                         glm::vec3(0.0f, -1.0f, 0.0f));
    is_moving_ = true;
  }

  meshes_[0].SetModelMatrix(model_);

  glm::mat4 front_left_leg(1.0f);
  front_left_leg =
      glm::translate(front_left_leg, glm::vec3(0.27226f, 0.40636f, -0.6673f));
  front_left_leg = glm::rotate(front_left_leg, glm::radians(rotation_),
                               glm::vec3(1.0f, 0.0f, 0.0f));
  front_left_leg = model_ * front_left_leg;
  meshes_[1].SetModelMatrix(front_left_leg);

  glm::mat4 front_right_leg(1.0f);
  front_right_leg =
      glm::translate(front_right_leg, glm::vec3(-0.32459f, 0.40636f, -0.6673f));
  front_right_leg = glm::rotate(front_right_leg, glm::radians(-rotation_),
                                glm::vec3(1.0f, 0.0f, 0.0f));
  front_right_leg = model_ * front_right_leg;
  meshes_[2].SetModelMatrix(front_right_leg);

  glm::mat4 back_left_leg(1.0f);
  back_left_leg =
      glm::translate(back_left_leg, glm::vec3(0.40658f, 0.57055f, 0.6641f));
  back_left_leg = glm::rotate(back_left_leg, glm::radians(-rotation_),
                              glm::vec3(1.0f, 0.0f, 0.0f));
  back_left_leg = model_ * back_left_leg;
  meshes_[3].SetModelMatrix(back_left_leg);

  glm::mat4 back_right_leg(1.0f);
  back_right_leg = glm::translate(back_right_leg, glm::vec3(-0.45, 0.57, 0.66));
  back_right_leg = glm::rotate(back_right_leg, glm::radians(rotation_),
                               glm::vec3(1.0f, 0.0f, 0.0f));
  back_right_leg = model_ * back_right_leg;
  meshes_[4].SetModelMatrix(back_right_leg);
}

void PolarBear::OnKeyboardDown(unsigned char key) {
  is_key_pressed_[key] = true;
}

void PolarBear::OnKeyboardUp(unsigned char key) {
  is_key_pressed_[key] = false;
  is_moving_ = false;
}
