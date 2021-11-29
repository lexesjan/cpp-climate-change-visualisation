#ifndef CLIMATE_CHANGE_VISUALISATION_BOID_H
#define CLIMATE_CHANGE_VISUALISATION_BOID_H

#include "animated_model.h"

struct Obstacle {
  glm::vec2 position;
  float radius;
};

class Boid {
 public:
  explicit Boid(glm::vec2 position, AnimatedModel& model,
                float platform_radius);

  void UpdatePosition(std::vector<Boid>& others);

  void Draw();

  void SetDelta(float delta);

  void SetObstacles(std::shared_ptr<std::vector<Obstacle>> obstacles);

 private:
  glm::vec2 position_;
  glm::vec2 velocity_;
  glm::vec2 acceleration_;
  AnimatedModel model_;
  float max_speed_;
  float max_force_;
  float delta_;
  float platform_radius_;
  std::shared_ptr<std::vector<Obstacle>> obstacles_;

  const glm::vec2 GetAlignment(std::vector<Boid>& boids) const;

  const glm::vec2 GetCohesion(std::vector<Boid>& boids) const;

  const glm::vec2 GetSeparation(std::vector<Boid>& boids) const;

  const glm::vec2 GetObstacleSeparation() const;

  const glm::vec2 GetEdges(float radius) const;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_BOID_H