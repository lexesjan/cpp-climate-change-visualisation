#ifndef CLIMATE_CHANGE_VISUALISATION_PLAYER_H_
#define CLIMATE_CHANGE_VISUALISATION_PLAYER_H_

#include "animated_model.h"

class Player : public AnimatedModel {
 public:
  explicit Player(std::string path, Shader &shader, Renderer &renderer,
                  Material = Material(glm::vec3(0.0f)));

  void UpdatePosition();

  void OnKeyboardDown(unsigned char key);

  void OnKeyboardUp(unsigned char key);

  void Draw() override;

 private:
  std::vector<bool> is_key_pressed_;
  glm::mat4 model_matrix_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_PLAYER_H_
