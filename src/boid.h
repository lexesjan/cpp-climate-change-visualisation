#ifndef CLIMATE_CHANGE_VISUALISATION_BOID_H
#define CLIMATE_CHANGE_VISUALISATION_BOID_H

#include "animated_model.h"

class Boid {
 public:
  explicit Boid(glm::vec2 position, AnimatedModel& model,
                float vision_radius_ = 10.0f);

  void UpdatePosition(std::vector<Boid>& others,
                      std::vector<glm::vec2>& obstacles, float radius);

  void Draw();

  void SetDelta(float delta);

 private:
  glm::vec2 position_;
  glm::vec2 velocity_;
  AnimatedModel model_;
  float vision_radius_;
  float max_speed_;
  float delta_;

  const std::vector<Boid> GetNeighbours(std::vector<Boid>& boids) const;

  const glm::vec2 GetAlignment(std::vector<Boid>& boids) const;

  const glm::vec2 GetCohesion(std::vector<Boid>& boids) const;

  const glm::vec2 GetSeparation(std::vector<Boid>& boids) const;

  const glm::vec2 GetObstacleSeparation(
      std::vector<glm::vec2>& obstacles) const;

  const glm::vec2 GetEdges(float radius) const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_BOID_H