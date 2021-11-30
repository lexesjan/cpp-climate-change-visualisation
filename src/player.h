#ifndef CLIMATE_CHANGE_VISUALISATION_PLAYER_H_
#define CLIMATE_CHANGE_VISUALISATION_PLAYER_H_

#include "animated_model.h"
#include "boid.h"

class Player : public AnimatedModel {
 public:
  explicit Player(std::string path, Shader &shader, Renderer &renderer,
                  float player_speed = 1.0f,
                  Material = Material(glm::vec3(0.0f)));

  void UpdatePosition();

  void OnKeyboardDown(unsigned char key);

  void OnKeyboardUp(unsigned char key);

  void Draw() override;

  const glm::vec3 GetPosition() const;

  void SetPosition(glm::vec3 &position);

  void SetDelta(float delta) override;

  void SetObstacles(std::shared_ptr<std::vector<Obstacle>> obstacles);

 private:
  std::vector<bool> is_key_pressed_;
  glm::mat4 model_matrix_;
  float player_speed_;
  std::shared_ptr<std::vector<Obstacle>> obstacles_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_PLAYER_H_
