#ifndef CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_
#define CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_

#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "directed_light.h"
#include "light_source.h"
#include "boid.h"
#include "skybox.h"
#include "particle_system.h"

class Application {
 public:
  explicit Application();

  void Init();

  void Display();

  void UpdateScene();

  void OnMouseMove(int x, int y);

  void OnKeyboardDown(unsigned char key, int x, int y);

  void OnKeyboardUp(unsigned char key, int x, int y);

 private:
  Renderer renderer_;
  Camera camera_;
  Shader animated_model_shader_;
  Shader basic_shader_;
  Shader model_shader_;
  Shader skybox_shader_;
  Shader particle_shader_;
  DirectedLight directed_light_;
  std::vector<LightSource> campfires_;
  Player player_;
  Model platform_;
  Model rock_;
  std::vector<Boid> boids_;
  std::vector<ModelPosition> rock_positions_;
  Skybox skybox_;
  ParticleSystem particle_system_;
  ParticleProps smoke_particle_props_;
  bool pause_crowd_;
  float last_time_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_
