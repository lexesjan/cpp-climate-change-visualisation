#ifndef CLIMATE_CHANGE_VISUALISATION_BOID_H
#define CLIMATE_CHANGE_VISUALISATION_BOID_H

#include "animated_model.h"

class Boid {
 public:
  explicit Boid(glm::vec2 position, AnimatedModel& model);

  void UpdatePosition(std::vector<Boid>& others,
                      std::vector<glm::vec2>& obstacles, float radius);

  void Draw();

  void SetDelta(float delta);

 private:
  glm::vec2 position_;
  glm::vec2 velocity_;
  glm::vec2 acceleration_;
  AnimatedModel model_;
  float max_speed_;
  float max_force_;
  float delta_;

  const glm::vec2 GetAlignment(std::vector<Boid>& boids) const;

  const glm::vec2 GetCohesion(std::vector<Boid>& boids) const;

  const glm::vec2 GetSeparation(std::vector<Boid>& boids) const;

  const glm::vec2 GetObstacleSeparation(
      std::vector<glm::vec2>& obstacles) const;

  const glm::vec2 GetEdges(float radius) const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_BOID_H