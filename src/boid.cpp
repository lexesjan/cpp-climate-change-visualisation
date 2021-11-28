#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <iostream>
#include "boid.h"
#include "random_utils.h"
#include "glm_utils.h"

Boid::Boid(glm::vec2 position, AnimatedModel& model, float vision_radius)
    : position_(position),
      velocity_(random_utils::range(-1.0f, 1.0f),
                random_utils::range(-1.0f, 1.0f)),
      model_(model),
      vision_radius_(vision_radius),
      max_speed_(1.0f),
      delta_(0.0f) {
  velocity_ = glm::normalize(velocity_);
}

void Boid::UpdatePosition(std::vector<Boid>& others,
                          std::vector<glm::vec2>& obstacles, float radius) {
  glm::vec2 alignment = GetAlignment(others);
  glm::vec2 cohesion = GetCohesion(others);
  glm::vec2 separation = GetSeparation(others);
  glm::vec2 obstacle_separation = GetObstacleSeparation(obstacles);

  cohesion = glm_utils::set_length(cohesion, 0.5f);

  velocity_ += alignment;
  velocity_ += cohesion;
  velocity_ += separation;
  velocity_ += obstacle_separation;

  velocity_ = glm_utils::limit(velocity_, max_speed_);

  position_ += velocity_ * delta_;

  model_.UpdateBoneTransformations();
}

void Boid::Draw() {
  glm::mat4 model_matrix = glm::translate(
      glm::mat4(1.0f), glm::vec3(position_.x, 0.0f, position_.y));

  float rotation = glm::atan(velocity_.x, velocity_.y);

  model_matrix =
      glm::rotate(model_matrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f));

  model_.GetShader().SetUniformMatrix4fv("model", GL_FALSE,
                                         glm::value_ptr(model_matrix));
  model_.Draw();
}

void Boid::SetDelta(float delta) {
  delta_ = delta;
  model_.SetDelta(delta);
}

const std::vector<Boid> Boid::GetNeighbours(std::vector<Boid>& boids) const {
  std::vector<Boid> neighbours;

  for (Boid& boid : boids) {
    float distance = glm::distance(boid.position_, position_);

    if (&boid != this && distance < vision_radius_) {
      neighbours.push_back(boid);
    }
  }

  return neighbours;
}

const glm::vec2 Boid::GetAlignment(std::vector<Boid>& boids) const {
  const std::vector<Boid>& neighbours = GetNeighbours(boids);

  glm::vec2 average(0.0f);

  for (const Boid& boid : neighbours) {
    float distance = glm::distance(boid.position_, position_);

    glm::vec2 copy = boid.velocity_;
    copy = glm::normalize(copy);
    copy /= distance;

    average += copy;
  }

  return average;
}

const glm::vec2 Boid::GetCohesion(std::vector<Boid>& boids) const {
  const std::vector<Boid>& neighbours = GetNeighbours(boids);

  glm::vec2 average(0.0f);

  for (const Boid& boid : neighbours) {
    average += boid.position_;
  }

  if (neighbours.size() > 0) {
    average /= neighbours.size();

    glm::vec2 steering = average - position_;

    return steering;
  }

  return glm::vec2(0.0f);
}

const glm::vec2 Boid::GetSeparation(std::vector<Boid>& boids) const {
  const std::vector<Boid>& neighbours = GetNeighbours(boids);

  glm::vec2 average(0.0f);

  for (const Boid& boid : neighbours) {
    float distance = glm::distance(boid.position_, position_);

    glm::vec2 difference = position_ - boid.position_;
    difference = glm::normalize(difference);
    difference /= distance;

    average += difference;
  }

  return average;
}

const glm::vec2 Boid::GetObstacleSeparation(
    std::vector<glm::vec2>& obstacles) const {
  glm::vec2 average(0.0f);

  for (glm::vec2& obstacle : obstacles) {
    float distance = glm::distance(obstacle, position_);

    if (distance > 0) {
      glm::vec2 difference = position_ - obstacle;
      difference = glm::normalize(difference);
      difference /= distance;

      average += difference;
    }
  }

  return average;
}
