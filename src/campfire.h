#ifndef CLIMATE_CHANGE_VISUALISATION_CAMPFIRE_H_
#define CLIMATE_CHANGE_VISUALISATION_CAMPFIRE_H_

#include "light_source.h"
#include "particle_system.h"

class Campfire : public LightSource {
 public:
  explicit Campfire(std::string path, glm::vec3 colour, glm::vec3 position,
                    Shader& shader, Renderer& renderer,
                    ParticleSystem& particle_system, float constant = 1.0f,
                    float linear = 0.5f, float quadratic = 0.05f,
                    Material material = Material(glm::vec3(0.0f),
                                                 glm::vec3(0.0f),
                                                 glm::vec3(0.0f)));

  void Draw() override;

 private:
  ParticleSystem& particle_system_;
  ParticleProps particle_props_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_CAMPFIRE_H_
