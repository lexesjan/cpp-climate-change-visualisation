#ifndef CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_
#define CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_

#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "directed_light.h"
#include "light_source.h"

class Application {
 public:
  explicit Application();

  void Display();

  void UpdateScene();

  void OnMouseMove(int x, int y);

  void OnKeyboardDown(unsigned char key, int x, int y);

  void OnKeyboardUp(unsigned char key, int x, int y);

 private:
  Renderer renderer_;
  Camera camera_;
  Shader animated_model_shader_;
  Shader light_source_shader_;
  Shader model_shader_;
  Player player_;
  Model monkey_head_;
  DirectedLight directed_light_;
  LightSource campfire_;
  LightSource cube_;
  float last_time_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_
