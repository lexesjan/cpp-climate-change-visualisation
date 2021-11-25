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
  DirectedLight directed_light_;
  std::vector<LightSource> campfires_;
  Player player_;
  Model platform_;
  Model rock_;
  float last_time_;
};

#endif  // CLIMATE_CHANGE_VISUALISATION_APPLICATION_H_
