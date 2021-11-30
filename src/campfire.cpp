#include "campfire.h"

Campfire::Campfire(std::string path, glm::vec3 colour, glm::vec3 position,
                   Shader& shader, Renderer& renderer,
                   ParticleSystem& particle_system, float constant,
                   float linear, float quadratic, Material material)
    : particle_system_(particle_system),
      particle_props_(),
      LightSource(path, colour, position, shader, renderer, constant, linear,
                  quadratic, material) {
  particle_props_.colour_begin = glm::vec4(colour, 1.0f);
  particle_props_.colour_end =
      glm::vec4(117.0f / 255.0f, 118.0f / 255.0f, 118.0f / 255.0f, 1.0f);
  particle_props_.life_time = 1.0f;
  particle_props_.position =
      glm::vec3(position.x, position.y + 0.25f, position.z);
  particle_props_.size_begin = 0.1f;
  particle_props_.size_end = 0.05f;
  particle_props_.size_variation = 0.5f;
  particle_props_.velocity = glm::vec3(0.0f, 1.5f, 0.0f);
  particle_props_.velocity_variation = glm::vec3(3.0f);
}

void Campfire::Draw() {
  particle_system_.Emit(particle_props_);

  LightSource::Draw();
}
