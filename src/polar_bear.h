#ifndef CLIMATE_CHANGE_VISUALISATION_POLAR_BEAR_H_
#define CLIMATE_CHANGE_VISUALISATION_POLAR_BEAR_H_

#include <vector>
#include "mesh.h"

class PolarBear {
 public:
  explicit PolarBear();

  const std::vector<Mesh> &GetMeshes() const;

  void SetDelta(float delta);

  void UpdatePosition();

  void OnKeyboardDown(unsigned char key);

  void OnKeyboardUp(unsigned char key);

 private:
  float delta_;
  float rotation_;
  float d_rotation_;
  bool is_moving_;
  glm::mat4 model_;
  std::vector<bool> is_key_pressed_;
  std::vector<Mesh> meshes_;
};

#endif
