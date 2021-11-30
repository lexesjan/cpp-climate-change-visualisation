#ifndef CLIMATE_CHANGE_VISUALISATION_PARTICLE_SYSTEM_H_
#define CLIMATE_CHANGE_VISUALISATION_PARTICLE_SYSTEM_H_

#include <vector>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include "shader.h"
#include "renderer.h"

struct ParticleProps {
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 velocity_variation;
  glm::vec4 colour_begin;
  glm::vec4 colour_end;
  float size_begin;
  float size_end;
  float size_variation;
  float life_time;
};

class ParticleSystem {
 public:
  explicit ParticleSystem(Shader& shader, Renderer& renderer);

  void Emit(ParticleProps& props);

  void SetDelta(float delta);

  void UpdatePosition();

  void Draw();

 private:
  struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 colour_begin;
    glm::vec4 colour_end;
    float size_begin;
    float size_end;
    float life_time;
    float life_remaining;
    float rotation;
    bool is_active;
  };

  std::vector<Particle> particle_pool_;
  unsigned int pool_index_;
  float delta_;
  Shader shader_;
  Renderer renderer_;
  std::vector<glm::vec3> vertices_;
  VertexArrayObject vertex_array_object_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_PARTICLE_SYSTEM_H_