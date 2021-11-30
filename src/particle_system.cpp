#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle_system.h"
#include "random_utils.h"

ParticleSystem::ParticleSystem(Shader& shader, Renderer& renderer)
    : particle_pool_(),
      pool_index_(999),
      delta_(0.0f),
      shader_(shader),
      renderer_(renderer_),
      vertex_array_object_(),
      vertices_({
          -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
          0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

          -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

          -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

          0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
          0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

          -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
          0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

          -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
      }) {
  particle_pool_.resize(1000);

  VertexBufferObject vertex_buffer_object(
      vertices_.data(),
      (GLsizei)(vertices_.size() * sizeof(vertices_.front())));

  VertexBufferLayout vertex_buffer_layout;
  vertex_buffer_layout.AddElement<float>(3);

  vertex_array_object_.AddBuffer(vertex_buffer_object, vertex_buffer_layout);
}

void ParticleSystem::Emit(ParticleProps& props) {
  Particle& particle = particle_pool_[pool_index_];
  particle.is_active = true;
  particle.position = props.position;
  particle.rotation = random_utils::range(2.0f * glm::pi<float>());

  // Velocity.
  particle.velocity = props.velocity;
  particle.velocity.x += random_utils::range(-props.velocity_variation.x / 2,
                                             props.velocity_variation.x / 2);
  particle.velocity.y += random_utils::range(-props.velocity_variation.y / 2,
                                             props.velocity_variation.y / 2);
  particle.velocity.z += random_utils::range(-props.velocity_variation.z / 2,
                                             props.velocity_variation.z / 2);

  // Colour.
  particle.colour_begin = props.colour_begin;
  particle.colour_end = props.colour_end;

  // Lifetime.
  particle.life_time = props.life_time;
  particle.life_remaining = props.life_time;
  particle.size_begin =
      props.size_begin +
      random_utils::range(-props.size_variation / 2, props.size_variation / 2);
  particle.size_end = props.size_end;

  pool_index_ = --pool_index_ % particle_pool_.size();
}

void ParticleSystem::SetDelta(float delta) { delta_ = delta; }

void ParticleSystem::UpdatePosition() {
  for (Particle& particle : particle_pool_) {
    if (!particle.is_active) {
      continue;
    }

    if (particle.life_remaining <= 0.0f) {
      particle.is_active = false;

      continue;
    }

    particle.life_remaining -= delta_;
    particle.position += particle.velocity * delta_;
    particle.rotation += 0.01f * delta_;
  }
}

void ParticleSystem::Draw() {
  for (Particle& particle : particle_pool_) {
    if (!particle.is_active) {
      continue;
    }

    // Decay particles.
    float life = particle.life_remaining / particle.life_time;
    glm::vec4 colour =
        glm::lerp(particle.colour_end, particle.colour_begin, life);
    colour.a *= life;

    float size = glm::lerp(particle.size_end, particle.size_begin, life);

    // Calculate model matrix.
    glm::mat4 model_mat(1.0f);
    model_mat = glm::translate(model_mat, particle.position);
    model_mat =
        glm::rotate(model_mat, particle.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    model_mat = glm::scale(model_mat, glm::vec3(size, size, size));

    // Set uniforms.
    shader_.SetUniformMatrix4fv("model", GL_FALSE, glm::value_ptr(model_mat));
    shader_.SetUniform4f("colour", colour);

    renderer_.Draw(vertex_array_object_, shader_, (GLsizei)vertices_.size());
  }
}
