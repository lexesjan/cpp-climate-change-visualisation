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
  std::vector<bool> is_key_pressed_;
  std::vector<Mesh> meshes_;
  float delta_;
  float rotation_;
  glm::mat4 model_;
};

#endif
