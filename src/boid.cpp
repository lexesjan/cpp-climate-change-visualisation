#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <iostream>
#include "boid.h"
#include "random_utils.h"
#include "glm_utils.h"

Boid::Boid(glm::vec2 position, AnimatedModel& model)
    : position_(position),
      velocity_(random_utils::range(-1.0f, 1.0f),
                random_utils::range(-1.0f, 1.0f)),
      acceleration_(0.0f),
      model_(model),
      max_speed_(2.0f),
      max_force_(0.001f),
      delta_(0.0f) {
  velocity_ = glm::normalize(velocity_);
}

void Boid::UpdatePosition(std::vector<Boid>& others,
                          std::vector<glm::vec2>& obstacles, float radius) {
  // Calculate acceleration.
  glm::vec2 alignment = GetAlignment(others);
  glm::vec2 cohesion = GetCohesion(others);
  glm::vec2 separation = GetSeparation(others);
  glm::vec2 obstacle_separation = GetObstacleSeparation(obstacles);
  glm::vec2 edges = GetEdges(radius);

  acceleration_ += alignment;
  acceleration_ += cohesion;
  acceleration_ += separation * 5.0f;
  acceleration_ += obstacle_separation * 5.0f;
  acceleration_ += edges * 5.0f;

  // Move boid position.
  velocity_ += acceleration_;
  velocity_ = glm_utils::limit(velocity_, max_speed_);
  position_ += velocity_ * delta_;

  acceleration_ *= 0;

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

const glm::vec2 Boid::GetAlignment(std::vector<Boid>& boids) const {
  float neighbour_radius = 10.0f;
  unsigned int count = 0;

  glm::vec2 sum(0.0f);

  for (Boid& boid : boids) {
    float distance = glm::distance(position_, boid.position_);

    if (distance > 0 && distance < neighbour_radius) {
      sum += boid.velocity_;
      count++;
    }
  }

  if (count > 0) {
    glm::vec2 desired = sum / (float)count;
    desired = glm_utils::set_length(desired, max_speed_);

    glm::vec2 steering = desired - velocity_;
    steering = glm_utils::limit(steering, max_force_);

    return steering;
  }

  return glm::vec2(0.0f);
}

const glm::vec2 Boid::GetCohesion(std::vector<Boid>& boids) const {
  float neighbour_radius = 10.0f;
  unsigned int count = 0;

  glm::vec2 sum(0.0f);

  for (Boid& boid : boids) {
    float distance = glm::distance(position_, boid.position_);

    if (distance > 0 && distance < neighbour_radius) {
      sum += boid.position_;
      count++;
    }
  }

  if (count > 0) {
    sum /= count;

    glm::vec2 desired = sum - position_;
    desired = glm_utils::set_length(desired, max_speed_);

    glm::vec2 steering = desired - velocity_;
    steering = glm_utils::limit(steering, max_force_);

    return steering;
  }

  return glm::vec2(0.0f);
}

const glm::vec2 Boid::GetSeparation(std::vector<Boid>& boids) const {
  float separation_radius = 5.0f;
  unsigned int count = 0;

  glm::vec2 steering(0.0f);

  for (Boid& boid : boids) {
    float distance = glm::distance(position_, boid.position_);

    if (distance > 0 && distance < separation_radius) {
      glm::vec2 difference = position_ - boid.position_;
      difference = glm::normalize(difference);
      difference /= distance;

      steering += difference;
      count++;
    }
  }

  if (count > 0) {
    steering /= count;
  }

  if (glm::length(steering) > 0) {
    glm::vec2 desired = glm_utils::set_length(steering, max_speed_);

    steering = desired - velocity_;
    steering = glm_utils::limit(steering, max_force_);
  }

  return steering;
}

const glm::vec2 Boid::GetObstacleSeparation(
    std::vector<glm::vec2>& obstacles) const {
  float separation_radius = 5.0f;
  unsigned int count = 0;

  glm::vec2 steering(0.0f);

  for (glm::vec2& obstacle : obstacles) {
    float distance = glm::distance(position_, obstacle);

    if (distance > 0 && distance < separation_radius) {
      glm::vec2 difference = position_ - obstacle;
      difference = glm::normalize(difference);
      difference /= distance;

      steering += difference;
      count++;
    }
  }

  if (count > 0) {
    steering /= count;
  }

  if (glm::length(steering) > 0) {
    glm::vec2 desired = glm_utils::set_length(steering, max_speed_);

    steering = desired - velocity_;
    steering = glm_utils::limit(steering, max_force_);
  }

  return steering;
}

const glm::vec2 Boid::GetEdges(float radius) const {
  glm::vec2 steering(0.0f);

  float separation_radius = 5.0f;
  float distance = glm::distance(position_, glm::vec2(0.0f));

  if (distance > 0 && distance > radius - separation_radius) {
    glm::vec2 difference = glm::vec2(0.0f) - position_;
    difference = glm::normalize(difference);
    difference /= distance;

    steering += difference;
  }

  if (glm::length(steering) > 0) {
    glm::vec2 desired = glm_utils::set_length(steering, max_speed_);

    steering = desired - velocity_;
    steering = glm_utils::limit(steering, max_force_);
  }

  return steering;
}
