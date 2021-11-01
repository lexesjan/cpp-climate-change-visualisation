#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "polar_bear.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

PolarBear::PolarBear() {
  delta_ = 0.0f;
  is_key_pressed_ = std::vector<bool>(256, false);

  meshes_.push_back(Mesh("models/polar_bear_body.dae"));
  meshes_.push_back(Mesh("models/polar_bear_front_left_leg.dae"));
  meshes_.push_back(Mesh("models/polar_bear_front_right_leg.dae"));
  meshes_.push_back(Mesh("models/polar_bear_bottom_left_leg.dae"));
  meshes_.push_back(Mesh("models/polar_bear_bottom_right_leg.dae"));
}

const std::vector<Mesh>& PolarBear::GetMeshes() const { return meshes_; }

void PolarBear::SetDelta(float delta) { delta_ = delta; }

void PolarBear::UpdatePosition() {
  glm::mat4 model = meshes_[0].GetModelMatrix();

  if (is_key_pressed_['i']) {
    model = glm::translate(model, delta_ * glm::vec3(0.0f, 0.0f, -5.0f));
  }

  if (is_key_pressed_['k']) {
    model = glm::translate(model, delta_ * glm::vec3(0.0f, 0.0f, 5.0f));
  }

  if (is_key_pressed_['j']) {
    model = glm::rotate(model, glm::radians(180.0f) * delta_,
                        glm::vec3(0.0f, 1.0f, 0.0f));
  }

  if (is_key_pressed_['l']) {
    model = glm::rotate(model, glm::radians(180.0f) * delta_,
                        glm::vec3(0.0f, -1.0f, 0.0f));
  }

  meshes_[0].SetModelMatrix(model);

  for (unsigned int i = 1; i < meshes_.size(); ++i) {
    glm::mat4 modelChild = meshes_[i].GetModelMatrix();

    modelChild = model * modelChild;

    meshes_[i].SetModelMatrix(modelChild);
  }
}

void PolarBear::OnKeyboardDown(unsigned char key) {
  is_key_pressed_[key] = true;
}

void PolarBear::OnKeyboardUp(unsigned char key) {
  is_key_pressed_[key] = false;
}
